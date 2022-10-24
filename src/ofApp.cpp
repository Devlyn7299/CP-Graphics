#include "ofApp.h"
#include "buildTerrainMesh.h"

//void buildCube(ofMesh& mesh)
//{
//    using namespace glm;
//
//    // Generate the cube mesh
//    mesh.addVertex(vec3(-1, -1, -1)); // front left bottom
//    mesh.addVertex(vec3(-1, -1, 1)); // back  left bottom
//    mesh.addVertex(vec3(-1, 1, -1)); // front left top
//    mesh.addVertex(vec3(-1, 1, 1));  // back  left top
//    mesh.addVertex(vec3(1, -1, -1)); // front right bottom
//    mesh.addVertex(vec3(1, -1, 1));  // back  right bottom
//    mesh.addVertex(vec3(1, 1, -1));  // front right top
//    mesh.addVertex(vec3(1, 1, 1));   // back  right top
//
//    ofIndexType indices[36] =
//    {
//        2, 3, 6, 7, 6, 3, // top
//        0, 4, 1, 1, 4, 5, // bottom
//        0, 1, 2, 2, 1, 3, // left
//        4, 6, 5, 5, 6, 7, // right
//        0, 2, 4, 4, 2, 6, // front
//        1, 5, 3, 3, 5, 7, // back
//    };
//
//    mesh.addIndices(indices, 36);
//
//    mesh.flatNormals();
//
//    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
//    {
//        mesh.setNormal(i, -mesh.getNormal(i));
//    }
//}
//
//void buildCircle(ofMesh& mesh, int subdiv)
//{
//    using namespace glm;
//
//    // Populate with vertex positions.
//
//    // Center
//    mesh.addVertex(vec3(0));
//
//    // Outer ring
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        double theta{ i * (2 * PI) / subdiv };
//        mesh.addVertex(vec3(cos(theta), sin(theta), 0));
//    }
//
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        mesh.addIndex(0);
//        mesh.addIndex(1 /* outer ring start */ + i);
//        mesh.addIndex(1 /* outer ring start */ + (i + 1) % subdiv);
//    }
//
//    mesh.flatNormals();
//
//    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
//    {
//        mesh.setNormal(i, -mesh.getNormal(i));
//    }
//}
//
//void buildCylinder(ofMesh& mesh, int subdiv)
//{
//    using namespace glm;
//
//    // Populate with vertex positions.
//
//    // Top center
//    mesh.addVertex(vec3(0, 1, 0)); // vertex 0
//
//    // Top outer ring -- vertices 1 through subdiv
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        double theta{ i * (2 * PI) / subdiv };
//        mesh.addVertex(vec3(cos(theta), 1, -sin(theta)));
//    }
//
//    // Bottom center
//    mesh.addVertex(vec3(0, -1, 0)); // vertex subdiv + 1
//
//    // Bottom outer ring -- vertices subdiv + 2 through 2 * subdiv + 1
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        double theta{ i * (2 * PI) / subdiv };
//        mesh.addVertex(vec3(cos(theta), -1, -sin(theta)));
//    }
//
//    // Start of index buffer: populate to form faces
//
//    // Top circle
//    int topRingStart{ 1 };
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        mesh.addIndex(0); // top center
//        mesh.addIndex(topRingStart + i);
//        mesh.addIndex(topRingStart + (i + 1) % subdiv);
//    }
//
//    // Bottom circle
//    int bottomRingStart{ subdiv + 2 };
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        mesh.addIndex(subdiv + 1); // bottom center
//
//        // Swap order from top circle so that the circle faces downwards instead of upwards.
//        mesh.addIndex(bottomRingStart + (i + 1) % subdiv);
//        mesh.addIndex(bottomRingStart + i);
//    }
//
//    // Sides
//    for (int i{ 0 }; i < subdiv; i++)
//    {
//        // Define the indices of a single quad
//        int indices[4]
//        {
//            topRingStart + i,
//            topRingStart + (i + 1) % subdiv,
//            bottomRingStart + i,
//            bottomRingStart + (i + 1) % subdiv
//        };
//
//        // build the quad
//        mesh.addIndex(indices[0]);
//        mesh.addIndex(indices[2]);
//        mesh.addIndex(indices[1]);
//        mesh.addIndex(indices[1]);
//        mesh.addIndex(indices[2]);
//        mesh.addIndex(indices[3]);
//    }
//
//    mesh.flatNormals();
//
//    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
//    {
//        mesh.setNormal(i, -mesh.getNormal(i));
//    }
//}
//
//void buildSphere(ofMesh& mesh, int subdivTheta, int subdivPhi)
//{
//    using namespace glm;
//
//    // Populate with vertex positions.
//
//    // Top center (north pole)
//    mesh.addVertex(vec3(0, 1, 0)); // vertex 0
//
//    // Rings
//    // Start at i = 1 since i=0 is the north pole
//    for (int i{ 1 }; i < subdivTheta; i++)
//    {
//        double theta{ i * PI / subdivTheta };
//
//        for (int j{ 0 }; j < subdivPhi; j++)
//        {
//            double phi{ j * (2 * PI) / subdivPhi };
//
//            mesh.addVertex(vec3(sin(theta) * cos(phi), cos(theta), -sin(theta) * sin(phi)));
//        }
//    }
//
//    // Bottom center (south pole)
//    mesh.addVertex(vec3(0, -1, 0)); // vertex ???
//
//
//    // Start of index buffer: populate to form faces
//
//    // Top circle (around north pole)
//    for (int i{ 0 }; i < subdivPhi; i++)
//    {
//        int topRingStart{ 1 };
//
//        mesh.addIndex(0); // top center / north pole
//        mesh.addIndex(topRingStart + i);
//        mesh.addIndex(topRingStart + (i + 1) % subdivPhi);
//    }
//
//    int southPole{ (subdivTheta - 1) * subdivPhi + 1 };
//
//    // Bottom circle (around south pole)
//    for (int i{ 0 }; i < subdivPhi; i++)
//    {
//        int bottomRingStart{ southPole - subdivPhi };
//
//        mesh.addIndex(southPole); // bottom center / south pole
//
//        // Swap order from top circle so that the circle faces downwards instead of upwards.
//        mesh.addIndex(bottomRingStart + (i + 1) % subdivPhi);
//        mesh.addIndex(bottomRingStart + i);
//    }
//
//    // Sides
//    for (int i{ 0 }; i < subdivTheta - 2; i++)
//    {
//        for (int j{ 0 }; j < subdivPhi; j++)
//        {
//            int topRingStart{ 1 + subdivPhi * i };
//            int bottomRingStart{ 1 + subdivPhi * (i + 1) };
//
//            // Define the indices of a single quad
//            int indices[4]
//            {
//                topRingStart + j,
//                topRingStart + (j + 1) % subdivPhi,
//                bottomRingStart + j,
//                bottomRingStart + (j + 1) % subdivPhi
//            };
//
//            // build the quad
//            mesh.addIndex(indices[0]);
//            mesh.addIndex(indices[2]);
//            mesh.addIndex(indices[1]);
//            mesh.addIndex(indices[1]);
//            mesh.addIndex(indices[2]);
//            mesh.addIndex(indices[3]);
//        }
//    }
//
//    mesh.flatNormals();
//
//    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
//    {
//        mesh.setNormal(i, -mesh.getNormal(i));
//    }
//}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex(); // IMPORTANT!

    ofEnableDepthTest();

    glEnable(GL_CULL_FACE); // Enable face culling to test winding order

    // Load shaders for the first time
    reloadShaders();

    // Load torus mesh
    //torusMesh.load("torus.ply");



    ofSetBackgroundColor(ofColor(135, 206, 235));

    heightmapHiRes.setUseTexture(false);
    heightmapHiRes.load("TamrielHighRes.png");
    heightmap.setUseTexture(false);
    // 641 x 513
    heightmap.load("TamrielLowRes.png");
    assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

    water.addVertex(glm::vec3(0, 700, 0));
    water.addVertex(glm::vec3(0, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, 0));

    ofIndexType indices[6] = { 0,1,2,2,3,0 };
    water.addIndices(indices, 6);


     //Build terrain mesh
    //buildTerrainMesh(terrainMesh, heightmapHiRes, (heightmapHiRes.getWidth() - 1) * 0.5f - 640, (heightmapHiRes.getHeight() - 1) * 0.5f - 640,
    //    (heightmapHiRes.getWidth() - 1) * 0.5f + 640, (heightmapHiRes.getHeight() - 1) * 0.5f + 640, //heightmap.getWidth() - 1, heightmap.getHeight() - 1,
    //    glm::vec3(1, 50, 1));

    buildTerrainMesh(terrainMesh, heightmap, 0, 0,
        heightmap.getWidth() - 1, heightmap.getHeight() - 1,
        glm::vec3(32, 50*32, 32));


        //glm::vec3((heightmapHiRes.getHeight()-1)/(heightmap.getHeight()-1)));
    //buildTerrainMesh(terrainMesh, heightmap, 0, 0,
    //    heightmap.getWidth() - 1, heightmap.getHeight() - 1,
    //    glm::vec3((1, 50, 1)));
    glm::vec3 newPosition = glm::vec3(0, 50, 0);
    //newPosition = glm::vec3(
    //    (heightmap.getWidth() - 1),
    //    50,
    //    (heightmap.getHeight() - 1)
    //);    
    //assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);
    //position = glm::vec3(
    //    (heightmapHiRes.getWidth() - 1) * 0.5f,
    //    10, (heightmapHiRes.getHeight() - 1) * 0.5f);

    //newPosition = glm::vec3(140, 25, 100);

    newPosition = glm::vec3(
        ((heightmap.getWidth() - 1) * 0.5f) * 32,
        25 * 32,
        ((heightmap.getHeight() - 1) * 0.5f) * 32
    );

    position = newPosition;
    //cout << position << endl;


    cellManager.initializeForPosition(position);


    //buildTerrainMesh(terrainMeshHigh, heightmap, 0, 0,
    //    heightmap.getWidth() - 1, heightmap.getHeight() - 1,
    //    glm::vec3(1, 50, 1));
    
    //// Generate cube mesh
    //buildCube(cubeMesh);

    //// Generate circle mesh
    //buildCircle(circleMesh, 40);

    //// Generate cylinder mesh
    //buildCylinder(cylinderMesh, 8);

    //buildSphere(sphereMesh, 4, 8);
}

void ofApp::reloadShaders()
{
    shader.load("shader.vert", "shader.frag");
    waterShader.load("shader.vert", "shader.frag");
    needsReload = false;
}

void ofApp::updateCameraRotation(float dx, float dy)
{
    using namespace glm;
    cameraHead += dx;
    cameraPitch += dy;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (needsReload)
    {
        reloadShaders();
    }

    using namespace glm;

    // calculate world space velocity
    vec3 velocityWorldSpace{ mat3(rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0))) * velocity};

    // time since last frame
    float dt{ static_cast<float>(ofGetLastFrameTime()) };

    // update position
    position += velocityWorldSpace * dt;

    cellManager.optimizeForPosition(position);

    //cout << position << endl;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    using namespace glm;
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    float nearPlane = 0.01f;
    float midPlane = 250.0f;
    float farPlane = 3000.0f;
    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect{ width / height };

    mat4 model{ rotate(radians(0.0f), vec3(1, 1, 1)) * scale(vec3(1,1,1)) };
    mat4 view{ (rotate(cameraPitch, vec3(1, 0, 0)) * rotate(cameraHead, vec3(0, 1, 0))) * translate(-position) };
    mat4 proj{ perspective(radians(90.0f), aspect, midPlane - 249.0f, farPlane) };

    mat4 mvp{ proj * view * model };
    mat4 modelView{ view * model };

    shader.begin(); // start using the shader
    shader.setUniformMatrix4f("mvp", mvp);
    shader.setUniformMatrix4f("modelView", modelView);
    shader.setUniformMatrix3f("normalMatrix", mat3(model));

    shader.setUniform1f("nearPlane", 0);
    shader.setUniform1f("midPlane", 0);
    shader.setUniform1f("needsSmoothstep", 0);

    shader.setUniform3f("meshColor", vec3(86.0f/255.0f, 125.0f/255.0f, 70.0f/255.0f));
    shader.setUniform3f("lightColor", vec3(0.5));
    shader.setUniform3f("lightDir", normalize(vec3(-1, 1, 1)));
    shader.setUniform3f("ambientColor", vec3(0.1, 0.2, 0.1));

    //cubeMesh.draw();
    terrainMesh.draw();

    shader.setUniform3f("meshColor", vec3(11.0f / 255.0f, 199.0f / 255.0f, 250.0f / 255.0f));

    water.draw();


    glClear(GL_DEPTH_BUFFER_BIT);

    proj = { perspective(radians(90.0f), aspect, nearPlane, midPlane + 250.0f) };
    mvp = { proj * view * model };


    shader.setUniformMatrix4f("mvp", mvp);



    shader.setUniform1f("nearPlane", nearPlane);
    shader.setUniform1f("midPlane", midPlane);
    shader.setUniform1f("needsSmoothstep", 1);
    water.draw();

    shader.setUniform3f("meshColor", vec3(86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f));

    cellManager.drawActiveCells(position, midPlane + 250.0f);

    //terrainMeshHigh.draw();
    shader.end(); // done with the shader
    ofDisableBlendMode();

}

void ofApp::exit()
{
    cellManager.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'w')
    {
        velocity.z = -50;
    }
    else if (key == 's')
    {
        velocity.z = 50;
    }
    else if (key == 'a')
    {
        velocity.x = -50;
    }
    else if (key == 'd')
    {
        velocity.x = 50;
    }

    // Added R and F to go up and down, relative to where the camera is pointing
    else if (key == 'f')
    {
        velocity.y = -50;
    }
    else if (key == 'r')
    {
        velocity.y = 50;
    }

    // Allowing the user to disable mouseMovement with E
    if (key == 'e')
    {
        allowMouseMovement = !allowMouseMovement;
    }

    if (key == '`')
    {
        needsReload = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == 'w' || key == 's')
    {
        velocity.z = 0;
    }
    else if (key == 'a' || key == 'd')
    {
        velocity.x = 0;
    }
    else if (key == 'f' || key == 'r')
    {
        velocity.y = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    if (allowMouseMovement)
    {
        if (prevX != 0 && prevY != 0)
        {
            // Update camera rotation based on mouse movement
            updateCameraRotation(mouseSensitivity * (x - prevX), mouseSensitivity * (y - prevY));
        }
    }

    // Remember where the mouse was this frame.
    prevX = x;
    prevY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
