#pragma once
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"

struct Lighting
{
public:
    glm::vec3 ambientLight;
    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;
};