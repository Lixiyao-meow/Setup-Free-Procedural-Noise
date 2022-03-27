#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Transform.h"

class LightSource: public Transform{
public:
	LightSource(std::vector<glm::vec3> LightPos, std::vector<glm::vec3> LightIntensity, 
				std::vector<glm::vec3> LightColor, float Att_const, 
				float Att_lin, float Att_quad){
		lightPos = LightPos;
		lightIntensity = LightIntensity;
		lightColor = LightColor;
		
        att_const = Att_const;
        att_lin = Att_lin;
        att_quad = Att_quad;
	};
	
	std::vector<glm::vec3> get_Pos(){return lightPos;}

	std::vector<glm::vec3> get_Intensity(){return lightIntensity;}

	std::vector<glm::vec3> get_Color(){return lightColor;}

	float get_Const(){return att_const;}

	float get_Lin(){return att_lin;}

    float get_quad(){return att_quad;}

private:
	std::vector<glm::vec3> lightPos;
	std::vector<glm::vec3> lightIntensity;
	std::vector<glm::vec3> lightColor;

	float att_const;
	float att_lin;
	float att_quad;
};

