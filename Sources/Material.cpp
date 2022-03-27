#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Material{
public:
	Material(glm::vec3 MatAlbedo, float Roughness, float Metallicness){
        matAlbedo = MatAlbedo;
		roughness = Roughness;
		metallicness = Metallicness;
	};
	
	glm::vec3 get_Albedo(){return matAlbedo;}

	float get_roughness(){return roughness;}

	float get_metallicness(){return metallicness;}

private:
	glm::vec3 matAlbedo;
	float roughness;
	float metallicness;
};

