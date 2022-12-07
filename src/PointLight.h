#pragma once
#include "ofMain.h"

struct PointLight
{
public:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};