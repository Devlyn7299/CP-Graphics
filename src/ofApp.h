#pragma once
#include "ofMain.h"
#include "Camera.h"
#include "ofxCubemap.h"
#include "CameraMatrices.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


private:
    // Shader for a directional light
    ofShader directionalLightShader;

    // Shader for a directional light
    ofShader pointLightShader;

    // Shader for a spot light
    ofShader spotLightShader;

    // Mesh of a shield
    ofMesh shieldMesh;
    ofMesh staffMesh;
    ofMesh jarMesh;
    ofMesh coneMesh;
    ofMesh cube;
    ofMesh cylinderMesh;
    ofMesh sphereMesh;
    ofMesh torusMesh;

    // diffuse (color) texture
    ofImage shieldDiffuse;

    // normal map
    ofImage shieldNormal;

    // specular texture
    ofImage shieldSpecular;

    // Mesh for the skybox
    ofMesh cubeMesh;

    // Cubemap texture
    ofxCubemap cubemap;

    // Shader for rendering the skybox with a cubemap
    ofShader skyboxShader;

    // 3D perspective shader
    ofShader shader;

    // mesh for the low resolution terrain
    ofMesh terrainMesh;

    // image used to build low resolution terrain mesh
    ofShortImage heightmap{};

    // image used to build high resolutino terrain mesh
    ofShortImage heightmapHiRes{};

    // mesh for the water
    ofMesh water;

    // CellManager used to build cells of the high resolution heightmap mesh
    CellManager<7> cellManager{ heightmapHiRes, 1600, 256 };

    // boolean that either allows or disallows mouse movement
    bool allowMouseMovement{ true };

    // Do shaders need to be reloaded?
    bool needsReload{ true };

    // Load the shaders for this app
    void reloadShaders();

    // (x, y) from the previous frame
    int prevX{ 0 }, prevY{ 0 };

    // How many radians of rotation correspond to a single pixel of movement of the cursor.
    float mouseSensitivity{ 0.02f };

    // The current head direction of the camera in radians.
    float cameraHead{ 0 };

    // The current pitch direction of the camera in radians
    float cameraPitch{ 0 };

    // update camera rotation based on mouse movement
    void updateCameraRotation(float dx, float dy);

    // Velocity of the camera (from WASD) -- in camera space
    glm::vec3 velocity{ };

    // Position of the camera in world space
    glm::vec3 position{ };

    // Exit function to be used for the cellManager when the program is closed
    void exit();
};
