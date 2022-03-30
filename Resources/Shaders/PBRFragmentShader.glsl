#version 450 core // Minimal GL version support expected from the GPU

#ifndef PI
#  define PI 3.14159265358979323846
#endif

in vec3 fPosition;
in vec3 fNormal; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)

out vec4 colorResponse; // Shader output: the color response attached to this fragment

uniform mat4 projectionMat, modelViewMat, normalMat;
uniform int sampler_size;
uniform vec3 PoissonSamples[4000];

struct Material {
	vec3 matAlbedo;
	float roughness;
	float metallicness;
}; 
  
uniform Material material;

struct LightSource {
	vec3 lightPos[3];
	vec3 lightIntensity[3];
	vec3 lightColor[3];
	
	float att_const;
	float att_lin;
	float att_quad;
};

uniform LightSource lightSource;

/* Functions for Real-Time Rendering */

vec3 brdf (vec3 matAlbedo, vec3 N, vec3 lightPos){
	return matAlbedo/3.14 * max(0.0,(dot(lightPos, N)));
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

/* Functions for Noise Generation*/

/* Projecting Poisson Disk on the tangent plan */

// return x and y position of the projected point and its weight
vec3 Poisson_projection(vec3 position, vec3 normal, vec3 point){
    vec3 v = point - position;
    float dist = v.x * normal.x + v.y * normal.y + v.z * normal.z;
    vec3 projected_point = point - dist * normal;
    
    // compute the projected point position on the tangent plan and its weight
    float x = projected_point[0];
    float y = projected_point[1];
    float dist_proj_pos = length(projected_point - position);
    if( dist_proj_pos < 0.1f ){ // consider only the points nearby
        float weight = 1 - abs(dist);
        return vec3(x, y, weight);
    }
    else return vec3(0,0,0);
}

/* Kernels for the convolution*/

/* 1. Isotropic kernel*/
// 1.1
float smooth_cosine_kernel(float x, float y){
    float dist = sqrt(x*x + y*y);
    return 0.5f + 0.5f * cos(PI * dist); 
}
// 1.2
float gabor(float K, float a, float F_0, float omega_0, float x, float y){
    float gaussian_envelop = K * exp( -PI * (a * a) * ( x*x + y*y) );
    float sinusoidal_carrier = cos(2.0 * PI * F_0 * ( x*cos(omega_0) - y*sin(omega_0) ));
    return gaussian_envelop * sinusoidal_carrier;
}

/* 2. Anisotropic kernel*/

// 2.1.implementation in the paper
mat2 sigma_F_inv(float sigma, mat2 J){
    mat2 matrix_J = J * transpose(J);
    return 4 * PI*PI * sigma*sigma * matrix_J;
}

mat2 sigma_G_inv(float a){
    mat2 I = mat2(1, 0,
                  0, 1);
    return 2*PI/(a*a) * I;
}

mat2 sigma_FG(mat2 sigma_F_inv, mat2 sigma_G_inv){
    return inverse( sigma_F_inv + sigma_F_inv );
}

vec2 mu_G(float F_0, float omega_0){
    return vec2 (F_0*cos(omega_0), F_0*sin(omega_0));
}

float a_(mat2 sigma_FG){
    return 2*PI*sqrt( determinant(sigma_FG) );
}

float K_(float K, float a, float a_, vec2 mu_G, mat2 sigma_F_inv, mat2 sigma_G_inv){
    vec2 mu = mu_G;
    mat2 sigma = inverse(sigma_F_inv) + inverse(sigma_G_inv);
    float pow = -0.5f * dot((-mu) * inverse(sigma), (-mu));
    float N = exp(pow);
    return K * (a_*a_)/(a*a) * N;
}

// 2.2 implementation with an anisotropic Gaussian envelop
float gabor_anisotropic(float sigma_x, float sigma_y, float F_0, float omega_0, float x, float y){
    float gaussian_envelop = 1/(2*PI*sigma_x*sigma_y) * exp(- x*x/(2*sigma_x*sigma_x)- y*y/(2*sigma_y*sigma_y) );
    float sinusoidal_carrier = cos(2.0 * PI * F_0 * ( x*cos(omega_0) - y*sin(omega_0) ));
    return gaussian_envelop * sinusoidal_carrier;
}

void main () {

    /* Noise Generator */

    // 1. Isotropic Gabor Kernel Parameters
    float K = 0.5f;
    float a = 0.05f;
    float F_0 = 8;
    float omega_0 = - PI / 4.0;

    // 2.1. Anisotropic Gabor Kernel Parameters in the paper
    mat2 J = mat2 (4.0, 4.0,
                   4.0, 5.0);
    float sigma = 1.0f;
    mat2 Sigma_F_inv = sigma_F_inv(sigma, J);
    mat2 Sigma_G_inv = sigma_G_inv(a);
    mat2 Sigma_FG = sigma_FG(Sigma_F_inv, Sigma_G_inv);
    float a_ = a_(Sigma_FG);
    vec2 Mu_G = mu_G(a, omega_0);
    float K_ = K_(K, a, a_, Mu_G, Sigma_F_inv, Sigma_G_inv);

    // 2.2. Gabor Kernel with an Anisotrpic Gaussian Envelop Parameters
    float sigma_x = 0.15;
    float sigma_y = 0.5;

    // Poisson Projection and Noise Generation

    float noise = 0;
    int nb_sample = 0;
    for(int i=0; i<sampler_size; i++){
        vec3 point = PoissonSamples[i];
        vec3 poisson_projection = Poisson_projection(fPosition, fNormal, point);
        
        float x = poisson_projection[0];
        float y = poisson_projection[1];
        float weight = poisson_projection[2];

        if( weight != 0){
            //noise += weight * smooth_cosine_kernel(x, y); // 1.1. smooth cosine kernel
            noise += weight * gabor(K, a, F_0, omega_0, x, y); // 1.2. Gabor kernel
            //noise += 4 * weight * gabor(K_, a_, F_0, omega_0, x, y); // 2.1. Gabor kernel with anisotropic filtering mentionned in paper
            //noise +=  weight * gabor_anisotropic(sigma_x, sigma_y, F_0, omega_0, x, y); // 2.2. Gabor kernel with an anisotropic Gaussian Envolop
        }
    }
    if(noise == 0)
        noise = 0;

    /* PBRF shader */
    
	vec3 N = normalize(fNormal);
    vec3 V = normalize(-fPosition);

    vec3 F0 = vec3(noise);

    F0 = mix(F0, material.matAlbedo, material.metallicness);

	// reflectance equation
    vec3 Lo = vec3(0.0);
    
    for(int i=0; i<3; i++){
        vec3 lightDir = normalize (lightSource.lightPos[i] - fPosition);
        vec3 L = normalize(vec3 (normalMat * vec4 (lightDir, 1.0)));
        //vec3 L = normalize(vec3 (vec4 (lightDir, 1.0)));
        vec3 H = normalize(V + L);
        float d = distance(lightSource.lightPos[i], fPosition);
        float attenuation = 1/(lightSource.att_const + lightSource.att_lin*d + lightSource.att_quad*d*d);
        vec3 radiance = lightSource.lightIntensity[i] * lightSource.lightColor[i] * attenuation;        
            
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, material.roughness);        
        float G   = GeometrySmith(N, V, L, material.roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
            
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.metallicness;	  
            
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
                
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * material.matAlbedo / PI + specular) * radiance * NdotL;
    }

	vec3 ambient = vec3(0.03) * material.matAlbedo;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

	colorResponse = vec4 (color, 1.0);
}