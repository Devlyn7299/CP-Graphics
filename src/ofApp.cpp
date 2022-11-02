#include "ofApp.h"
#include "CameraMatrices.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex(); // IMPORTANT!

    ofEnableDepthTest();

    // Load shaders for the first time
    reloadShaders();

    // Load torus mesh
    torusMesh.load("models/torus.ply");
    coneMesh.load("models/cone.ply");
    cubeMesh.load("models/cube.ply");
    cylinderMesh.load("models/cylinder.ply");
    sphereMesh.load("models/sphere.ply");
    
    // Set up scene graph
    sceneGraph.setup(torusMesh, coneMesh, cubeMesh, cylinderMesh, sphereMesh, shader);
}

void ofApp::reloadShaders()
{
    shader.load("shaders/my.vert", "shaders/my.frag");
    needsReload = false;
}

void ofApp::updateCameraRotation(float dx, float dy)
{
    using namespace glm;

    cameraPitch -= dy;
    // -89 to +89 degrees, converted to radians
    cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));

    cameraHead -= dx;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (needsReload)
    {
        reloadShaders();
    }

    using namespace glm;

    // Calculate world space velocity.
    vec3 velocityWorldSpace { mat3(rotate(cameraHead, vec3(0, 1, 0))) * velocity };

    // Time since last frame
    float dt { static_cast<float>(ofGetLastFrameTime()) };

    // Update position using velocity and dt.
    camera.position += velocityWorldSpace * dt;

    // Update rotation
    camera.rotation = rotate(cameraHead, vec3(0, 1, 0)) *
        rotate(cameraPitch, vec3(1, 0, 0));

    // Update scene graph
    sceneGraph.rootNode.updateSceneGraph(dt);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    using namespace glm;

    float width { static_cast<float>(ofGetViewportWidth()) };
    float height { static_cast<float>(ofGetViewportHeight()) };
    float aspect { width / height };

    // Calculate the view and projection matrices for the camera.
    CameraMatrices camMatrices { camera, aspect, 0.01f, 10.0f, };

    // Draw scene graph
    sceneGraph.rootNode.drawSceneGraph(camMatrices);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    using namespace glm;

    if (key == OF_KEY_UP || key == 'w')
    {
        // Forward motion
        velocity.z = -1;
    }
    else if (key == OF_KEY_DOWN || key == 's')
    {
        // Backwards motion
        velocity.z = 1;
    }
    else if (key == OF_KEY_LEFT || key == 'a')
    {
        // Forward motion
        velocity.x = -1;
    }
    else if (key == OF_KEY_RIGHT || key == 'd')
    {
        // Backwards motion
        velocity.x = 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == '`')
    {
        // Reload shaders
        needsReload = true;
    }
    else if (key == OF_KEY_LEFT || key == 'a' || key == OF_KEY_RIGHT || key == 'd')
    {
        // Reset velocity when a key is released
        velocity.x = 0;
    }
    else if (key == OF_KEY_UP || key == 'w' || key == OF_KEY_DOWN || key == 's')
    {
        // Reset velocity when a key is released
        velocity.z = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    if (prevX != 0 && prevY != 0)
    {
        // Previous mouse position has been initialized.
        // Calculate dx and dy
        int dx = x - prevX;
        int dy = y - prevY;

        // Update camera rotation based on mouse movement.
        updateCameraRotation(mouseSensitivity * dx, mouseSensitivity * dy);
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
