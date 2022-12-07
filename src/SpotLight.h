#pragma once
#include "ofMain.h"

struct SpotLight
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutoff;
    glm::vec3 color;
    float intensity;
};
