#include <iostream>
#include "PoissonDisk.h"

/* 	Create a sampler with the following parameters:
	width:  each sample's x coordinate will be between [0, width]
	height: each sample's y coordinate will be between [0, height]
	depth:  each sample's z coordinate will be between [0. depth]
	radius: each sample will be at least `radius` units away from any other sample, and at most 2 * `radius`.
*/

PoissonDisk::PoissonDisk(float width, float height, float depth, float radius0){
	cube = glm::vec3 (width, height, depth);
	radius  = radius0;
	cellSize = radius / sqrt (3);
	int Nx = ceil(width/cellSize), Ny = ceil(height/cellSize), Nz = ceil(depth/cellSize);
	grid.resize(Nx, Ny, Nz);

	for (int kx = 0; kx < Nx; ++kx) {
		for (int ky = 0; ky < Ny; ++ky) {
			for (int kz = 0; kz < Nz; ++kz) {
				grid(kx,ky,kz) = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

/// Return a lazy sequence of samples. You typically want to call this in a foreach loop, like so:
//  foreach (glm::vec3 sample in sampler.Samples()) { ... }
void PoissonDisk::Samples(){


	// First sample is choosen randomly
	if(activeSamples.size() == 0){
		AddSample(glm::vec3( ((double) rand() / (RAND_MAX)) * cube[0], 
							 ((double) rand() / (RAND_MAX)) * cube[1], 
							 ((double) rand() / (RAND_MAX)) * cube[2]));
	}
		
	while (activeSamples.size() > 0) {

		// Pick a random active sample
		int i = (int) ((double) rand() / (RAND_MAX)) * activeSamples.size();
		glm::vec3 sample = activeSamples[i];

		// Try `k` random candidates between [radius, 2 * radius] from that sample.
		bool found = false;
		for (int j = 0; j < k; ++j) {

			// Generate random point around sample.
			glm::vec3 candidate = GenerateRandomPointAround (sample, radius);

			// Accept candidates if it's inside the rect and farther than 2 * radius to any existing sample.
			if (IsContains (candidate, cube) && IsFarEnough(candidate)) {
				found = true;
				AddSample(candidate);
			}
		}		
		
		// If we couldn't find a valid candidate after k attempts, remove this sample from the active samples queue
		if (!found) {
			activeSamples[i] = activeSamples[activeSamples.size() - 1];
			activeSamples.pop_back();
		}
	}

}

bool PoissonDisk::IsContains (glm::vec3 v, glm::vec3 area) {
	if (v[0] >= 0 && v[0] < area[0] &&
	    v[1] >= 0 && v[1] < area[1] &&
		v[2] >= 0 && v[2] < area[2]) {
			return true;
	} 
	else {
		return false;
	}
}

glm::vec3 PoissonDisk::GenerateRandomPointAround (glm::vec3 point, float minDist){ //non-uniform, leads to denser packing.
	float r1 = ((double) rand() / (RAND_MAX)); //random point between 0 and 1
	float r2 = ((double) rand() / (RAND_MAX));
	float r3 = ((double) rand() / (RAND_MAX));
	//random radius between mindist and 2* mindist
	float radius = minDist * (r1 + 1);
	//random angle
	float angle1 = 2.0f * M_PI * r2;
	float angle2 = 2.0f * M_PI * r3;
	//the new point is generated around the point (x, y, z)
	float newX = point[0] + radius * std::cos (angle1) * std::sin (angle2);
	float newY = point[1] + radius * std::sin (angle1) * std::sin (angle2);
	float newZ = point[2] + radius * std::cos (angle2);

	return glm::vec3 (newX, newY, newZ);
}

bool PoissonDisk::IsFarEnough(glm::vec3 sample){
	
	GridPos pos = GridPos(sample, cellSize);
		
	int xmin = std::max (pos.x - 2, 0);
	int ymin = std::max (pos.y - 2, 0);
	int zmin = std::max (pos.z - 2, 0);
	int xmax = std::min (pos.x + 2, grid.w - 1);
	int ymax = std::min (pos.y + 2, grid.h - 1);
	int zmax = std::min (pos.z + 2, grid.l - 1);

	for (int z = zmin; z <= zmax; z++) {
		for (int y = ymin; y <= ymax; y++) {
			for (int x = xmin; x <= xmax; x++) {
				glm::vec3 s = grid(x, y, z);
				if (s != glm::vec3(0.0f, 0.0f, 0.0f)) {
					glm::vec3 d = s - sample;
					if (d.x * d.x + d.y * d.y + d.z * d.z < radius*radius) 
						return false;
				}
			}
		}
	}

	return true;
	
}

// Adds the sample to the active samples queue and the grid before returning it
void PoissonDisk::AddSample (glm::vec3 sample){
	activeSamples.push_back(sample);
	GridPos pos = GridPos(sample, cellSize);
	grid(pos.x, pos.y, pos.z) = sample;
}

std::vector<glm::vec3> PoissonDisk::output_samples(){
	std::vector<glm::vec3> output;
	int Nx = grid.w, Ny = grid.h, Nz = grid.l;
	for (int kx = 0; kx < Nx; ++kx) {
		for (int ky = 0; ky < Ny; ++ky) {
			for (int kz = 0; kz < Nz; ++kz) {
				if( grid(kx,ky,kz) != glm::vec3(0.0f, 0.0f, 0.0f) )
					output.push_back(grid(kx,ky,kz) - glm::vec3(cube[0]/2, cube[1]/2, cube[2]/2) );
			}
		}
	}	

	return output;
}