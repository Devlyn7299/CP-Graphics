#include "ofApp.h"
#include "CameraMatrices.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex(); // IMPORTANT!

    ofEnableDepthTest();

    // Load shaders for the first time
    reloadShaders();

    // Load meshes
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

    cameraPitch += dy;

    cameraHead += dx;
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
    vec3 velocityWorldSpace { mat3(rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0))) * velocity };

    // Time since last frame
    float dt { static_cast<float>(ofGetLastFrameTime()) };

    // Update position using velocity and dt.
    camera.position += velocityWorldSpace * dt;

    // Update rotation
    camera.rotation = rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0));

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

    if (key == 'w')
    {
        velocity.z = -1;
    }
    else if (key == 's')
    {
        velocity.z = 1;
    }
    else if (key == 'a')
    {
        velocity.x = -1;
    }
    else if (key == 'd')
    {
        velocity.x = 1;
    }

    // Added R and F to go up and down, relative to where the camera is pointing
    else if (key == 'f')
    {
        velocity.y = -1;
    }
    else if (key == 'r')
    {
        velocity.y = 1;
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
