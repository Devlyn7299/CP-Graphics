#pragma once
#include "ofMain.h"

struct DirectionalLight
{
public:
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};