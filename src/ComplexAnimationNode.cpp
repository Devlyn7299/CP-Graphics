#include "ComplexAnimationNode.h"

using namespace glm;

ComplexAnimationNode::ComplexAnimationNode(float rotationSpeed, vec3 rotationAxis, float maximumRotation)
    : rotationSpeed{ rotationSpeed }, rotationAxis{ rotationAxis }, maximumRotation{ maximumRotation }
{
}

void ComplexAnimationNode::updateNode(float dt, const mat4& model)
{
    // Update local transform by appending incremental rotation.
    this->localTransform = rotate(rotationSpeed * dt, rotationAxis) * this->localTransform;
    rotationTotal += rotationSpeed * dt;
    cout << "rotation " << rotationTotal << endl;
    if ((rotationTotal >= maximumRotation || rotationTotal <= -maximumRotation) && !needsToSwap)
    {
        needsToSwap = true;
        rotationSpeed = -rotationSpeed;
        cout << "shift ---------------------------------------" << endl;
    }

    if (needsToSwap && (rotationTotal < maximumRotation && rotationTotal > -maximumRotation))
    {
        needsToSwap = false;
    }
}
