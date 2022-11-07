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
    // RotationTotal keeps track of how far (in radians) an object has been rotated
    rotationTotal += rotationSpeed * dt;
    // If RotationTotal passes maximumRotation (or drops below negative maximumRotation), the direction of the rotation swaps
    // So the animation goes in a back-and-forth motion
    if ((rotationTotal >= maximumRotation || rotationTotal <= -maximumRotation) && !needsToSwap)
    {
        needsToSwap = true;
        rotationSpeed = -rotationSpeed;
    }

    // If The swap has started and rotationTotal went back into the range of maximumRotation, set needsToSwap to false
    if (needsToSwap && (rotationTotal < maximumRotation && rotationTotal > -maximumRotation))
    {
        needsToSwap = false;
    }
}
