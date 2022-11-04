#include "NewSceneGraph.h"
#include "SimpleDrawNode.h"
#include "SimpleAnimationNode.h"
// torusMesh, coneMesh, cubeMesh, cylinderMesh, sphereMesh
void NewSceneGraph::setup(const ofMesh& torusMesh, const ofMesh& coneMesh, const ofMesh& cubeMesh, const ofMesh& cylinderMesh,
    const ofMesh& sphereMesh, const ofShader& shader)
{
    using namespace glm;

    // Initialize scene graph
    rootNode.childNodes.emplace_back(new SimpleAnimationNode(0.0f, vec3(0, 0, 1)));

    // Animated node is the most recent node added to the scene graph
    meshAnimNode = rootNode.childNodes.back();

    // Attach cube mesh node to the animated node
    meshAnimNode->childNodes.emplace_back(new SimpleDrawNode{ cubeMesh, shader });

    // Cube mesh node is the most recent node added
    auto cubeMeshNode{ meshAnimNode->childNodes.back() };

    // Create joint node animation (right arm)
    cubeMeshNode->childNodes.emplace_back(new SimpleAnimationNode(2.0f, vec3(1, 0, 0)));
    auto armMesh{ cubeMeshNode->childNodes.back() };
    // Attach cylinder to animation
    armMesh->childNodes.emplace_back(new SimpleDrawNode{ cylinderMesh, shader });
    armMesh->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f));

    // Create joint node animation (right hand)
    armMesh->childNodes.emplace_back(new SimpleAnimationNode(4.0f, vec3(0, 1, 0)));
    auto handMesh{ armMesh->childNodes.back() };
    // Attach sphere to animation
    handMesh->childNodes.emplace_back(new SimpleDrawNode{ sphereMesh, shader });
    handMesh->localTransform = translate(vec3(0, 1, 0));

    // Duplicate entire right arm to left
    rootNode.childNodes.emplace_back(new SceneGraphNode());
    auto clone { rootNode.childNodes.back() };
    clone->localTransform = translate(vec3(-3.0f, 0, 0));
    clone->childNodes.push_back(armMesh);

    // Create neck animation
    cubeMeshNode->childNodes.emplace_back(new SimpleAnimationNode(2.0f, vec3(0, 1, 0)));
    auto neckMesh{ cubeMeshNode->childNodes.back() };
    // Attach torus to animation
    neckMesh->childNodes.emplace_back(new SimpleDrawNode{ torusMesh, shader });
    neckMesh->localTransform = translate(vec3(0, 1.25f, 0)) * scale(vec3(0.5f));

    // Create head animation
    cubeMeshNode->childNodes.emplace_back(new SimpleAnimationNode(-2.0f, vec3(0, 1, 0)));
    auto headMesh{ cubeMeshNode->childNodes.back() };
    // Attach cube to animation
    headMesh->childNodes.emplace_back(new SimpleDrawNode{ cubeMesh, shader });
    headMesh->localTransform = translate(vec3(0, 1.9f, 0)) * scale(vec3(0.5f));
    // * Ears - because why not
    headMesh->childNodes.emplace_back(new SimpleDrawNode{ sphereMesh, shader });
    auto rightEar{ headMesh->childNodes.back() };
    rightEar->localTransform = translate(vec3(1.25f, 0, 0)) * scale(vec3(0.5f));
    headMesh->childNodes.emplace_back(new SimpleDrawNode{ sphereMesh, shader });
    auto leftEar{ headMesh->childNodes.back() };
    leftEar->localTransform = translate(vec3(-1.25f, 0, 0)) * scale(vec3(0.5f));

}
