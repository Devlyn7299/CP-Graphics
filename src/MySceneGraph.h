#pragma once
#include "SceneGraphNode.h"

class MySceneGraph
{
public:
    void setup(const ofMesh& torusMesh, const ofShader& shader);

    // Root node of the scene graph
    SceneGraphNode rootNode {};

private:
    std::shared_ptr<SceneGraphNode> torusAnimNode;
};