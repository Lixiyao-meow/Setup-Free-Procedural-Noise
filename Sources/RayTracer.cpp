// ----------------------------------------------
// Polytechnique - INF584 "Image Synthesis"
//
// Base code for practical assignments.
//
// Copyright (C) 2022 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
#include "RayTracer.h"

#include "Console.h"
#include "Camera.h"


RayTracer::RayTracer() : 
	m_imagePtr (std::make_shared<Image>()) {}

RayTracer::~RayTracer() {}

void RayTracer::init (const std::shared_ptr<Scene> scenePtr) {
}

void RayTracer::render (const std::shared_ptr<Scene> scenePtr, Material material, LightSource lightSource) {
	size_t width = m_imagePtr->width();
	size_t height = m_imagePtr->height();
	size_t numOfMeshes = scenePtr->numOfMeshes ();
	std::chrono::high_resolution_clock clock;
	Console::print ("Start ray tracing at " + std::to_string (width) + "x" + std::to_string (height) + " resolution...");
	std::chrono::time_point<std::chrono::high_resolution_clock> before = clock.now();
	m_imagePtr->clear (scenePtr->backgroundColor ());
	
	// <---- Ray tracing code ---->
	
	// for each pifxel in image
	for(size_t i=0; i<width; i++){
		
		std::cout << float(i)/float(width) << std::endl;
		
		for(size_t j=0; j<height; j++){
			Camera camera;
			float x = float(i)/float(width);
			float y = 1 - float(j)/float(height);
			Ray ray = scenePtr->camera()->rayAt(x, y);

			float e = 100000.0f;
			// for each primitive in scene
			for(size_t k=0; k<numOfMeshes; k++){
				size_t N = scenePtr->mesh(k)->triangleIndices().size ();
				
				std::vector<glm::vec3> m_vertexPositions = scenePtr->mesh(k)->vertexPositions();
				std::vector<glm::vec3> m_vertexNormals = scenePtr->mesh(k)->vertexNormals();
	
				for(size_t n=0; n<N; n++){
					glm::vec3 v0 = m_vertexPositions[scenePtr->mesh(k)->triangleIndices()[n][0]];
					glm::vec3 v1 = m_vertexPositions[scenePtr->mesh(k)->triangleIndices()[n][1]];
					glm::vec3 v2 = m_vertexPositions[scenePtr->mesh(k)->triangleIndices()[n][2]];
					
					glm::vec3 n0 = m_vertexNormals[scenePtr->mesh(k)->triangleIndices()[n][0]];
					glm::vec3 n1 = m_vertexNormals[scenePtr->mesh(k)->triangleIndices()[n][1]];
					glm::vec3 n2 = m_vertexNormals[scenePtr->mesh(k)->triangleIndices()[n][2]];
					
					float b0, b1, b2, t;
					
					bool hit = rayTriangle(ray, v0, v1, v2, b0, b1, b2, t);
					if(hit){
						glm::vec3 triangle_barycenter = b0*v1 + b1*v2 + b2*v0;
						glm::vec3 triangle_normal = b0*n1 + b1*n2 + b2*n0;
						float d = glm::distance(ray.get_origin(), triangle_barycenter);
						if( d < e ){
							e = d;
							glm::vec3 color = ShadeColor(scenePtr, material, lightSource, triangle_barycenter, triangle_normal);
							m_imagePtr->set_color(i, j, color);
						}
					}
				}
			}			
		}
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> after = clock.now();
	double elapsedTime = (double)std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
	Console::print ("Ray tracing executed in " + std::to_string(elapsedTime) + "ms");
}

// test if a ray has an intersection with a triangle
// v0, v1, v2 the triangle's vertices
bool RayTracer::rayTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, 
							float &b0, float &b1, float &b2, float &t){
	glm::vec3 o = ray.get_origin();
	glm::vec3 w = ray.get_direction();

	glm::vec3 e0 = p1-p0;
	glm::vec3 e1 = p2-p0;
	glm::vec3 n = glm::normalize( glm::cross(e0,e1) );
	glm::vec3 q = glm::cross(w,e1);
	float a = glm::dot(e0,q);
	
	if(glm::dot(n,w)>=0 || abs(a)<0.0001f)
		return false;
	
	glm::vec3 s = (o-p0)/a;
	glm::vec3 r = glm::cross(s,e0);
	b0 = glm::dot(s,q);
	b1 = glm::dot(r,w);
	b2 = 1-b0-b1;
	if( b0<0 || b1<0 || b2<0 )
		return false;
	
	t = glm::dot(e1,r);

	if(t>=0)
		return true;
	
	return false;

}

glm::vec3 RayTracer::brdf(glm::vec3 matAlbedo, glm::vec3 N, glm::vec3 lightPos){
	return matAlbedo/3.14f * std::max(0.0f, glm::dot(lightPos, N));
}

glm::vec3 RayTracer::ShadeColor(const std::shared_ptr<Scene> scenePtr, Material material, LightSource lightSource, glm::vec3 vPosition, glm::vec3 vNormal){

	glm::mat4 modelViewMat = scenePtr->camera()->computeViewMatrix();
	glm::mat4 normalMat = inverse (modelViewMat);
	glm::vec4 p = modelViewMat * glm::vec4 (vPosition, 1.0);
    glm::vec4 n = normalMat * glm::vec4 (glm::normalize (vNormal), 1.0);
    glm::vec3 fNormal = glm::normalize (glm::vec3(n));
    glm::vec3 fPosition = glm::vec3(p);

	/*
	glm::vec3 color = glm::vec3(0);
	for(int i=0; i<3; i++){
        float d = glm::distance(lightSource.get_Pos()[i], fPosition);
        float attenuation = 1/(lightSource.get_Const() + lightSource.get_Lin()*d + lightSource.get_quad()*d*d);
        glm::vec3 radiance = lightSource.get_Intensity()[i] * lightSource.get_Color()[i] * attenuation;  
        color += brdf(material.get_Albedo(), fNormal, lightSource.get_Pos()[i]) * radiance;
    } */

	
	glm::vec3 N = glm::normalize(fNormal);
    glm::vec3 V = glm::normalize(-fPosition);

    glm::vec3 F0 = glm::vec3(0.04); 
    F0 = glm::mix(F0, material.get_Albedo(), material.get_metallicness());

	// reflectance equation
    glm::vec3 Lo = glm::vec3(0.0);
    
    for(int i=0; i<3; i++){
        glm::vec3 lightDir = glm::normalize (lightSource.get_Pos()[i] - fPosition);
        glm::vec3 L = glm::normalize(glm::vec3 (normalMat * glm::vec4 (lightDir, 1.0)));
        glm::vec3 H = glm::normalize(V + L);
        float d = glm::distance(lightSource.get_Pos()[i], fPosition);
        float attenuation = 1/(lightSource.get_Const() + lightSource.get_Lin()*d + lightSource.get_quad()*d*d);
        glm::vec3 radiance = lightSource.get_Intensity()[i]/(3.14f*3) * lightSource.get_Color()[i] * attenuation;        

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, material.get_roughness());        
        float G = GeometrySmith(N, V, L, material.get_roughness());      
        glm::vec3 F = fresnelSchlick(std::max(glm::dot(H, V), 0.0f), F0);       
            
        glm::vec3 kS = F;
        glm::vec3 kD = glm::vec3(1.0) - kS;
        kD *= 1.0 - material.get_metallicness();  
            
        glm::vec3 numerator = NDF * G * F;
        float denominator = 4.0f* std::max(glm::dot(N, V), 0.0f) * std::max(glm::dot(N, L), 0.0f) + 0.0001f;
        glm::vec3 specular = numerator / denominator;  
                
        // add to outgoing radiance Lo
        float NdotL = std::max(glm::dot(N, L), 0.0f);                
        Lo += (kD * material.get_Albedo() / 3.14f + specular) * radiance * NdotL;
    }

	glm::vec3 ambient = glm::vec3(0.03) * material.get_Albedo();
    glm::vec3 color = ambient + Lo;
	
    color = color / (color + glm::vec3(1.0));
    color = pow(color, glm::vec3(1.0/2.2));

	return color; 
}

glm::vec3 RayTracer::fresnelSchlick(float cosTheta, glm::vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
} 

float RayTracer::DistributionGGX(glm::vec3 N, glm::vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = std::max(glm::dot(N, H), 0.0f);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom       = 3.14f * denom * denom;

    return num / denom;
}

float RayTracer::GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float RayTracer::GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float roughness)
{
    float NdotV = std::max(glm::dot(N, V), 0.0f);
    float NdotL = std::max(glm::dot(N, L), 0.0f);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
