#include "MySceneGraph.h"
#include "LitDrawNode.h"
#include "SimpleAnimationNode.h"

void MySceneGraph::setup(const ofMesh& torusMesh, const ofShader& shader)
{
    using namespace glm;

    // Set up lighting first
    lighting.dirLight.direction = vec3(1, -1, -1);
    lighting.dirLight.color = vec3(0.5, 0.5, 1);
    lighting.dirLight.intensity = 0;
    lighting.ambientLight = vec3(0.1);
    lighting.spotLight.position = vec3(0, 0, 0);
    lighting.spotLight.direction = vec3(0, 0, -1);
    lighting.spotLight.cutoff = cos(radians(30.0f));
    lighting.spotLight.color = vec3(1, 1, 0.5);
    lighting.spotLight.intensity = 0;
    //lighting.pointLight.position = vec3(0, 0, 0);
    lighting.pointLight.color = vec3(1, 1, 0.5);
    lighting.pointLight.intensity = 1;

    // Initialize scene graph
    rootNode.childNodes.emplace_back(new SimpleAnimationNode(1.0f, vec3(1, 1, 1)));

    // Animated torus node is the most recent node added to the scene graph at this point
    torusAnimNode = rootNode.childNodes.back();

    // Attach torus mesh node to the animated node
    std::shared_ptr<LitDrawNode> torusMeshNode{ new LitDrawNode {torusMesh, shader, lighting} };
    torusAnimNode->childNodes.push_back(torusMeshNode);

    //torusAnimNode->childNodes.emplace_back(new LitDrawNode { torusMesh, shader, lighting });
    //// Torus mesh is most recent node added
    //auto torusMeshNode { torusAnimNode->childNodes.back() };

    // Set up lighting parameters for the torus
    //torusMeshNode->meshColor = vec3(1, 0.1f, 0.1f);

    // Create joint node
    torusAnimNode->childNodes.emplace_back(new SimpleAnimationNode { 2.0f, vec3(1, 0, 0) });
    auto jointNode { torusAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f));
    jointNode->childNodes.push_back(torusMeshNode);

    // Create joint node
    jointNode->childNodes.emplace_back(new SceneGraphNode());
    auto staticTranslation { jointNode->childNodes.back() };
    staticTranslation->localTransform = translate(vec3(1, 0, 0));

    staticTranslation->childNodes.emplace_back(new SimpleAnimationNode { 4.0f, vec3(0, 1, 0) });
    auto jointNode2 { staticTranslation->childNodes.back() };
    jointNode2->localTransform = translate(vec3(0.5f, 0, 0)) * scale(vec3(0.5f));
    jointNode2->childNodes.push_back(torusMeshNode);

    //auto prevTorusNode { torusAnimNode };

    //for (int i { 0 }; i < 5; i++)
    //{
    //    // Adding another copy of torus as child of previous node
    //    prevTorusNode->childNodes.emplace_back(new SceneGraphNode {});
    //    auto nextTorus { prevTorusNode->childNodes.back() };
    //    nextTorus->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f));
    //    nextTorus->childNodes.push_back(torusMeshNode);

    //    prevTorusNode = nextTorus;
    //}

    //// add a torus not part of the rotating chain
    //rootNode.childNodes.emplace_back(new SceneGraphNode {});
    //auto anotherTorus { rootNode.childNodes.back() };
    //anotherTorus->localTransform = translate(vec3(0, 0, 2));
    //anotherTorus->childNodes.push_back(torusMeshNode);

    //// Add a clone of the rotating chain
    //rootNode.childNodes.emplace_back(new SceneGraphNode {});
    //auto clone { rootNode.childNodes.back() };
    //clone->localTransform = translate(vec3(0, 0, -5));
    //clone->childNodes.push_back(torusAnimNode);
}
