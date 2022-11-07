#include "NewSceneGraph.h"
#include "LitDrawNode.h"
#include "SimpleAnimationNode.h"
#include "ComplexAnimationNode.h"

// torusMesh, coneMesh, cubeMesh, cylinderMesh, sphereMesh
void NewSceneGraph::setup(const ofMesh& torusMesh, const ofMesh& coneMesh, const ofMesh& cubeMesh, const ofMesh& cylinderMesh,
    const ofMesh& sphereMesh, const ofShader& shader)
{
    using namespace glm;

    // Set up lighting first
    lighting.dirLight.direction = vec3(1, -1, -1);
    lighting.dirLight.color = vec3(0.5, 0.5, 1);
    lighting.dirLight.intensity = 1;
    lighting.ambientLight = vec3(0.1);
    lighting.spotLight.position = vec3(1, 0, 2);
    lighting.spotLight.direction = vec3(0, 0, -1);
    lighting.spotLight.cutoff = cos(radians(30.0f));
    lighting.spotLight.color = vec3(1, 0.25, 0.25);
    lighting.spotLight.intensity = 1;
    lighting.pointLight.position = vec3(0, 0, -4);
    lighting.pointLight.color = vec3(1, 1, 0.5);
    lighting.pointLight.intensity = 0.25;



    // Initialize scene graph
    rootNode.childNodes.emplace_back(new SimpleAnimationNode(0.0f, vec3(0, 0, 1)));

    // Animated node is the most recent node added to the scene graph
    meshAnimNode = rootNode.childNodes.back();

    // Attach cube mesh node to the animated node
    meshAnimNode->childNodes.emplace_back(new LitDrawNode{ cubeMesh, shader, lighting });

    // Cube mesh node is the most recent node added
    auto cubeMeshNode{ meshAnimNode->childNodes.back() };

    // Create joint node animation (right arm)
    cubeMeshNode->childNodes.emplace_back(new ComplexAnimationNode(1.0f, vec3(1, 0, 0), 1.5f));
    auto armMesh{ cubeMeshNode->childNodes.back() };
    // Attach cylinder to animation
    armMesh->childNodes.emplace_back(new LitDrawNode{ cylinderMesh, shader, lighting });
    armMesh->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f, 1.0f, 0.5f));

    // Create joint node animation (right hand)
    armMesh->childNodes.emplace_back(new SimpleAnimationNode(4.0f, vec3(0, 1, 0)));
    auto handMesh{ armMesh->childNodes.back() };
    // Attach sphere to animation
    handMesh->childNodes.emplace_back(new LitDrawNode{ sphereMesh, shader, lighting });
    handMesh->localTransform = translate(vec3(0, -1, 0)) * scale(vec3(1.0f, 0.5f, 1.0f));

    // Duplicate entire right arm to left
    rootNode.childNodes.emplace_back(new SceneGraphNode());
    auto clone { rootNode.childNodes.back() };
    clone->localTransform = translate(vec3(-3.0f, 0, 0));
    clone->childNodes.push_back(armMesh);

    // Create neck animation
    cubeMeshNode->childNodes.emplace_back(new SimpleAnimationNode(2.0f, vec3(0, 1, 0)));
    auto neckMesh{ cubeMeshNode->childNodes.back() };
    // Attach torus to animation
    neckMesh->childNodes.emplace_back(new LitDrawNode{ torusMesh, shader, lighting });
    neckMesh->localTransform = translate(vec3(0, 1.25f, 0)) * scale(vec3(0.5f));

    // Create head animation
    cubeMeshNode->childNodes.emplace_back(new SimpleAnimationNode(-2.0f, vec3(0, 1, 0)));
    auto headMesh{ cubeMeshNode->childNodes.back() };
    // Attach cube to animation
    headMesh->childNodes.emplace_back(new LitDrawNode{ cubeMesh, shader, lighting });
    headMesh->localTransform = translate(vec3(0, 1.9f, 0)) * scale(vec3(0.5f));
    // * Ears - because why not
    headMesh->childNodes.emplace_back(new LitDrawNode{ sphereMesh, shader, lighting });
    auto rightEar{ headMesh->childNodes.back() };
    rightEar->localTransform = translate(vec3(1.25f, 0, 0)) * scale(vec3(0.5f));
    headMesh->childNodes.emplace_back(new LitDrawNode{ sphereMesh, shader, lighting });
    auto leftEar{ headMesh->childNodes.back() };
    leftEar->localTransform = translate(vec3(-1.25f, 0, 0)) * scale(vec3(0.5f));

    // Create joint node animation (right leg)
    cubeMeshNode->childNodes.emplace_back(new ComplexAnimationNode(1.0f, vec3(1, 0, 0), 0.5f));
    auto legMesh{ cubeMeshNode->childNodes.back() };
    // Attach cube to animation
    legMesh->childNodes.emplace_back(new LitDrawNode{ cubeMesh, shader, lighting });
    legMesh->localTransform = translate(vec3(0.5f, -2.0f, 0)) * scale(vec3(0.35f, 1.0f, 0.35f));

    // Create joint node animation (right leg)
    cubeMeshNode->childNodes.emplace_back(new ComplexAnimationNode(-1.0f, vec3(1, 0, 0), 0.5f));
    legMesh = { cubeMeshNode->childNodes.back() };
    // Attach cube to animation
    legMesh->childNodes.emplace_back(new LitDrawNode{ cubeMesh, shader, lighting });
    legMesh->localTransform = translate(vec3(-0.5f, -2.0f, 0)) * scale(vec3(0.35f, 1.0f, 0.35f));

    //// Duplicate entire right arm to left
    //rootNode.childNodes.emplace_back(new SceneGraphNode());
    //auto clone { rootNode.childNodes.back() };
    //clone->localTransform = translate(vec3(-1.0f, 0, 0));
    //clone->childNodes.push_back(legMesh);
}
