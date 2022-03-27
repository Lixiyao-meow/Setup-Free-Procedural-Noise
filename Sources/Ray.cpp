#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Transform.h"

class Ray{

public:
    Ray(glm::vec3 Origin, glm::vec3 Direction){
        origin = Origin;
        direction = Direction;
    };
    
    glm::vec3 get_origin(){return origin;}
    glm::vec3 get_direction(){return direction;}


private:
    glm::vec3 origin;
    glm::vec3 direction;
};