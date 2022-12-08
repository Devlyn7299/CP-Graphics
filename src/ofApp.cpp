#include "ofApp.h"
#include "buildTerrainMesh.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex(); // IMPORTANT!

    ofEnableDepthTest();

    // Enable face culling to test winding order
    glEnable(GL_CULL_FACE); 

    // Load shaders for the first time
    reloadShaders();

    // Setting the backround color to a sky blue color
    ofSetBackgroundColor(ofColor(135, 206, 235));   // Sky blue

    // Loading low resolution heightmap
    heightmap.setUseTexture(false);
    heightmap.load("TamrielLowRes.png");
    assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

    // Loading high resolution heightmap
    heightmapHiRes.setUseTexture(false);
    heightmapHiRes.load("TamrielHighRes.png");
    assert(heightmapHiRes.getWidth() != 0 && heightmapHiRes.getHeight() != 0);



    // Building the terrain mesh for the low resolution heightmap
    buildTerrainMesh(terrainMesh, heightmap, 0, 0,
        heightmap.getWidth() - 1, heightmap.getHeight() - 1,
        glm::vec3(32, 50*32, 32));

    // Setting the camera's position in the world
    glm::vec3 newPosition = glm::vec3(
        ((heightmap.getWidth() - 1) * 0.5f) * 32,
        30 * 32,
        ((heightmap.getHeight() - 1) * 0.5f) * 32
    );
    position = newPosition;

    // Building the terrain mesh for the high resolution heightmap (needs camera position)
    cellManager.initializeForPosition(position);

    // Creating the water mesh
    water.addVertex(glm::vec3(0, 700, 0));
    water.addVertex(glm::vec3(0, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, 0));

    ofIndexType indices[6] = { 0,1,2,2,3,0 };
    water.addIndices(indices, 6);
}

void ofApp::reloadShaders()
{
    shader.load("shader.vert", "shader.frag");
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
    using namespace glm;

    if (needsReload)
    {
        reloadShaders();
    }

    // calculate world space velocity
    vec3 velocityWorldSpace{ mat3(rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0))) * velocity};

    // time since last frame
    float dt{ static_cast<float>(ofGetLastFrameTime()) };

    // update position
    position += velocityWorldSpace * dt;

    // Update the cellManager to account for camera position changing
    cellManager.optimizeForPosition(position);

}

//--------------------------------------------------------------
void ofApp::draw()
{
    using namespace glm;

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    // Setting variables for the near, mid, and far planes
    float nearPlane = 1.0f;
    float midPlane = 250.0f;
    float farPlane = 3000.0f;

    // Setting variables for the aspect ratio of the window
    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect{ width / height };

    // Setting initial model, view, and projection matrices
    mat4 model{ rotate(radians(0.0f), vec3(1, 1, 1)) * scale(vec3(1,1,1)) };
    mat4 view{ (rotate(cameraPitch, vec3(1, 0, 0)) * rotate(cameraHead, vec3(0, 1, 0))) * translate(-position) };
    // First projection matrix is set for the low resolution texture (mid plane to far plane)
    mat4 proj{ perspective(radians(90.0f), aspect, midPlane - 249.0f, farPlane) };

    // Setting initial mvp and modelView matrices
    mat4 mvp{ proj * view * model };
    mat4 modelView{ view * model };

    shader.begin(); // start using the shader

    // Setting uniform variables for the vertex shader
    shader.setUniformMatrix4f("mvp", mvp);
    shader.setUniformMatrix4f("modelView", modelView);
    shader.setUniformMatrix3f("normalMatrix", mat3(model));

    // Setting uniform variables for smoothstep use in the fragment shader
    shader.setUniform1f("nearPlane", 0);
    shader.setUniform1f("midPlane", 0);
    shader.setUniform1f("needsSmoothstep", 0);

    // Setting uniform variables for lighting use in the fragment shader
    shader.setUniform3f("meshColor", vec3(86.0f/255.0f, 125.0f/255.0f, 70.0f/255.0f));  // Grass green
    shader.setUniform3f("lightColor", vec3(0.5));
    shader.setUniform3f("lightDir", normalize(vec3(-1, 1, 1)));
    shader.setUniform3f("ambientColor", vec3(0.1, 0.2, 0.1));

    // Drawing the low resolution terrain mesh and mid-to-far plane water
    terrainMesh.draw();
    shader.setUniform3f("meshColor", vec3(11.0f / 255.0f, 199.0f / 255.0f, 250.0f / 255.0f));   // Water
    water.draw();
    
    // Clearing depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Setting new projection for near-to-mid plane
    proj = { perspective(radians(90.0f), aspect, nearPlane, midPlane + 500.0f) };
    mvp = { proj * view * model };

    shader.setUniformMatrix4f("mvp", mvp);

    // Updating uniform variables for smoothstep use in the fragment shader
    shader.setUniform1f("nearPlane", nearPlane);
    shader.setUniform1f("midPlane", midPlane + 500.0f);
    shader.setUniform1f("needsSmoothstep", 1);

    // Drawing the near-to-mid plane water and high resolution terrain mesh
    water.draw();
    shader.setUniform3f("meshColor", vec3(86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f));    // Grass green
    cellManager.drawActiveCells(position, midPlane + 250.0f);

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
