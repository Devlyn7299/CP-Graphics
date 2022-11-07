#pragma once
#include "SceneGraphNode.h"

class ComplexAnimationNode : public SceneGraphNode
{
public:
    ComplexAnimationNode(float rotationSpeed, glm::vec3 rotationAxis, float maximumRotation);

protected:
    virtual void updateNode(float dt, const glm::mat4& model) override;

private:
    float rotationSpeed;
    glm::vec3 rotationAxis;
    float rotationTotal = 0;
    float maximumRotation;
    bool needsToSwap{ false };
};