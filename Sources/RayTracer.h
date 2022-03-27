// ----------------------------------------------
// Polytechnique - INF584 "Image Synthesis"
//
// Base code for practical assignments.
//
// Copyright (C) 2022 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
#pragma once

#include <random>
#include <cmath>
#include <algorithm>
#include <limits>
#include <memory>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Image.h"
#include "Scene.h"
#include "Material.cpp"
#include "LightSource.cpp"

using namespace std;

class RayTracer : public Transform{
public:
	
	RayTracer();
	virtual ~RayTracer();

	inline void setResolution (int width, int height) { m_imagePtr = make_shared<Image> (width, height); }
	inline std::shared_ptr<Image> image () { return m_imagePtr; }
	void init (const std::shared_ptr<Scene> scenePtr);
	void render (const std::shared_ptr<Scene> scenePtr, Material material, LightSource lightSource);
	bool rayTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, 
					 float &b0, float &b1, float &b2, float &t);
	
	glm::vec3 brdf(glm::vec3 matAlbedo, glm::vec3 N, glm::vec3 lightPos);
	
	glm::vec3 ShadeColor(const std::shared_ptr<Scene> scenePtr, Material material, LightSource LightSource, glm::vec3 fPosition, glm::vec3 fNormal);
	
	glm::vec3 fresnelSchlick(float cosTheta, glm::vec3 F0);
	float DistributionGGX(glm::vec3 N, glm::vec3 H, float roughness);
	float GeometrySchlickGGX(float NdotV, float roughness);
	float GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float roughness);


private:
	std::shared_ptr<Image> m_imagePtr;
};