#include "NewSceneGraph.h"
#include "SimpleDrawNode.h"
#include "SimpleAnimationNode.h"

void NewSceneGraph::setup(const ofMesh& mesh1, const ofMesh& mesh2, const ofMesh& mesh3, const ofMesh& mesh4, const ofMesh& mesh5, const ofShader& shader)
{
    using namespace glm;

    //// Initialize scene graph
    //rootNode.childNodes.emplace_back(new SimpleAnimationNode(1.0f, vec3(0, 0, 1)));

    //// Animated torus node is the most recent node added to the scene graph at this point
    //meshAnimNode = rootNode.childNodes.back();

    //// Attach torus mesh node to the animated node
    //meshAnimNode->childNodes.emplace_back(new SimpleDrawNode { mesh, shader });

    //// Torus mesh is most recent node added
    //auto meshNode { meshAnimNode->childNodes.back() };

    //// Create joint node
    //meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode { 2.0f, vec3(1, 0, 0) });
    //auto jointNode { meshAnimNode->childNodes.back() };
    //jointNode->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f));
    //jointNode->childNodes.push_back(meshNode);

    //// Create joint node
    //jointNode->childNodes.emplace_back(new SceneGraphNode());
    //auto staticTranslation { jointNode->childNodes.back() };
    //staticTranslation->localTransform = translate(vec3(1, 0, 0));

    //staticTranslation->childNodes.emplace_back(new SimpleAnimationNode { 4.0f, vec3(0, 1, 0) });
    //auto jointNode2 { staticTranslation->childNodes.back() };
    //jointNode2->localTransform = translate(vec3(0.5f, 0, 0)) * scale(vec3(0.5f));
    //jointNode2->childNodes.push_back(meshNode);

    // Body
    rootNode.childNodes.emplace_back(new SimpleAnimationNode(0.0f, vec3(0, 0, 1)));
    meshAnimNode = rootNode.childNodes.back();
    meshAnimNode->childNodes.emplace_back(new SimpleDrawNode{ mesh3, shader });
    auto meshNode { meshAnimNode->childNodes.back() };

    // Head
    meshAnimNode->childNodes.emplace_back(new SimpleDrawNode{ mesh1, shader });
    meshNode = meshAnimNode->childNodes.back();

    meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 0.0f, vec3(1, 0, 0) });
    auto jointNode { meshAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(0, 1.5f, 0)) * scale(vec3(0.5f));
    jointNode->childNodes.push_back(meshNode);

    jointNode->childNodes.emplace_back(new SceneGraphNode());
    auto staticTranslation{ jointNode->childNodes.back() };
    staticTranslation->localTransform = translate(vec3(0, 1.5f, 0));

    staticTranslation->childNodes.emplace_back(new SimpleAnimationNode{ 0.0f, vec3(0, 0, 1) });
    auto jointNode2{ staticTranslation->childNodes.back() };
    jointNode2->localTransform = scale(vec3(0.5f));
    jointNode2->childNodes.push_back(meshNode);


    // Left leg
    meshAnimNode->childNodes.emplace_back(new SimpleDrawNode{ mesh3, shader });
    meshNode = meshAnimNode->childNodes.back();

    meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 0.0f, vec3(1, 0, 0) });
    jointNode = { meshAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(0.75f, -1.5f, 0)) * scale(vec3(0.3f, 1.0f, 0.3f));
    jointNode->childNodes.push_back(meshNode);

    // Right leg
    meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 0.0f, vec3(1, 0, 0) });
    jointNode = { meshAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(-0.75f, -1.5f, 0)) * scale(vec3(0.3f, 1.0f, 0.3f));
    jointNode->childNodes.push_back(meshNode);


    // Left arm
    //meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 2.0f, vec3(1, 1, 0) });
    meshAnimNode->childNodes.emplace_back(new SimpleDrawNode{ mesh4, shader });
    meshNode = meshAnimNode->childNodes.back();

    meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 1.0f, vec3(1, 0, 0) });
    jointNode = { meshAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f, 1.0f, 0.5f));
    jointNode->childNodes.push_back(meshNode);

    jointNode->childNodes.emplace_back(new SceneGraphNode());
    staticTranslation = { jointNode->childNodes.back() };
    staticTranslation->localTransform = translate(vec3(0, -1, 0));

    staticTranslation->childNodes.emplace_back(new SimpleAnimationNode{ 4.0f, vec3(0, 0, 1) });
    jointNode2 = { staticTranslation->childNodes.back() };
    jointNode2->localTransform = scale(vec3(0.5f));
    jointNode2->childNodes.push_back(meshNode);

    // Right Arm
    meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 1.0f, vec3(1, 0, 0) });
    jointNode={ meshAnimNode->childNodes.back() };
    jointNode->localTransform = translate(vec3(-1.5f, 0, 0)) * scale(vec3(0.5f, 1.0f, 0.5f));
    jointNode->childNodes.push_back(meshNode);

    jointNode->childNodes.emplace_back(new SceneGraphNode());
    staticTranslation={ jointNode->childNodes.back() };
    staticTranslation->localTransform = translate(vec3(0, -1, 0));

    staticTranslation->childNodes.emplace_back(new SimpleAnimationNode{ 4.0f, vec3(0, 0, 1) });
    jointNode2={ staticTranslation->childNodes.back() };
    jointNode2->localTransform = scale(vec3(0.5f));
    jointNode2->childNodes.push_back(meshNode);



    //jointNode->childNodes.emplace_back(new SceneGraphNode());
    //staticTranslation = { jointNode->childNodes.back() };
    //staticTranslation->localTransform = translate(vec3(0, 1, 0));

    //staticTranslation->childNodes.emplace_back(new SimpleAnimationNode{ 4.0f, vec3(0, 0, 1) });
    //jointNode2 = { staticTranslation->childNodes.back() };
    //jointNode2->localTransform = scale(vec3(0.5f));
    //jointNode2->childNodes.push_back(meshNode);


    //meshAnimNode->childNodes.emplace_back(new SimpleAnimationNode{ 2.0f, vec3(1, 0, 0) });
    //jointNode = { meshAnimNode->childNodes.back() };
    //jointNode->localTransform = translate(vec3(-1.5f, 0, 0)) * scale(vec3(0.5f, 1.0f, 0.5f));
    //jointNode->childNodes.push_back(meshNode);



    //auto prevTorusNode { meshAnimNode };

    //for (int i { 0 }; i < 5; i++)
    //{
    //    // Adding another copy of torus as child of previous node
    //    prevTorusNode->childNodes.emplace_back(new SceneGraphNode {});
    //    auto nextTorus { prevTorusNode->childNodes.back() };
    //    nextTorus->localTransform = translate(vec3(1.5f, 0, 0)) * scale(vec3(0.5f));
    //    nextTorus->childNodes.push_back(meshNode);

    //    prevTorusNode = nextTorus;
    //}

    //// add a torus not part of the rotating chain
    //rootNode.childNodes.emplace_back(new SceneGraphNode {});
    //auto anotherTorus { rootNode.childNodes.back() };
    //anotherTorus->localTransform = translate(vec3(0, 0, 2));
    //anotherTorus->childNodes.push_back(meshNode);

    //// Add a clone of the rotating chain
    //rootNode.childNodes.emplace_back(new SceneGraphNode {});
    //auto clone { rootNode.childNodes.back() };
    //clone->localTransform = translate(vec3(0, 0, -5));
    //clone->childNodes.push_back(meshAnimNode);
}
