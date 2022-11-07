#pragma once
#include "SceneGraphNode.h"
#include "Lighting.h"

class NewSceneGraph
{
public:
    void setup(const ofMesh& mesh1, const ofMesh& mesh2, const ofMesh& mesh3, const ofMesh& mesh4, const ofMesh& mesh5, const ofShader& shader);

    // Root node of the scene graph
    SceneGraphNode rootNode {};

//private:
    std::shared_ptr<SceneGraphNode> meshAnimNode;
    Lighting lighting{};   // Lighting for the scene
};