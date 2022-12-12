#pragma once
#include "ofMain.h"
<<<<<<< Updated upstream
=======
#include "World.h"
#include "CellManager.h"
#include "Camera.h"
#include "CharacterPhysics.h"
#include "ofxXboxController.h"
#include "CameraMatrices.h"
>>>>>>> Stashed changes

struct HeightmapConfig
{
public:
    std::string hiResName;
    std::string loResName;
    float scale;
    float waterHeight;
    float gravity;
};

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
<<<<<<< Updated upstream
=======
    void exit();
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
    // 3D perspective shader
    ofShader shader;

    // Torus mesh object
    ofMesh torusMesh;

    // Cube mesh object
    ofMesh cubeMesh;

    // Circle mesh object
    ofMesh circleMesh;

    // Cylinder mesh object
    ofMesh cylinderMesh;

    // Sphere mesh object
    ofMesh sphereMesh;

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

    float cameraPitch{ 0 };

    // Velocity of the camera (from WASD) -- in camera space
    glm::vec3 velocity{ };

    // Position of the camera in world space
    glm::vec3 position{ 0, 10, 0 };

    // update camera rotation based on mouse movement
    void updateCameraRotation(float dx, float dy);

    ofMesh terrainMesh;

    bool allowMouseMovement{ true };


    //ofShortPixels heightmap;
=======
    // The number of quads in each row and column of a single terrain cell for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_SIZE { 256 };

    // The number of cells beyond the current cell that the cell manager loads for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_RANGE { 4 };

    const HeightmapConfig tamriel { "TamrielBeta_10_2016_01.png", "TamrielLowRes.png", 1600.0f, 0.4375f, 0.05f };
    //const HeightmapConfig random { "RandomHiRes.png", "RandomLowRes.png", 800.0f, 0.5f, 0.1f };

    HeightmapConfig config{ tamriel };

    // The vertical scale of the heightmap.  Used to convert the pixel values in the heightmap to physical height units.
    float heightmapScale { config.scale };

    // Non-GPU image containing the heightmap.
    ofShortImage heightmap {};

    // Plane mesh for rendering water.
    ofMesh waterPlane {};

    // Shader for rendering terrain.
    ofShader terrainShader {};

    // Shader for rendering water.
    ofShader waterShader {};

    // Shader for a directional light
    ofShader directionalLightShader;

    // Plane shader
    ofShader shadowShader{};

    // The main game "world" that uses the heightmap.
    World world {};

    // A cell manager for the high level-of-detail close terrain.
    CellManager<NEAR_LOD_RANGE + 1> cellManager { heightmap, heightmapScale, NEAR_LOD_SIZE };


    // Mesh of a shield
    ofMesh shieldMesh;

    // diffuse (color) texture
    ofImage shieldDiffuse;

    // normal map
    ofImage shieldNormal;

    // specular texture
    ofImage shieldSpecular;


    // A single terrain mesh. Uncomment the following line if not using a cell manager.
    ofMesh farTerrain {};
    ofVbo farTerrainVBO {};

    // Set to true when the shader reload hotkey is pressed.
    bool needsReload { true };

    // The first-person camera.
    Camera fpCamera {};

    // The helper object for physics calculations related to the first person character.
    //CharacterPhysics character { world };

    // The camera's "look" sensitivity when using the mouse.
    float camSensitivity { 0.02f };

    // The camera's "look" sensitivity when using an Xbox controller.
    float xboxCamSensitivity { 7.0f };

    // The character's walk speed (will be calculated based on the world's gravity).
    float characterWalkSpeed;

    // The character's jump speed (will be calculated based on the world's gravity).
    float characterJumpSpeed;

    // The first-person camera's head rotation.
    float headAngle { 0 };

    // The first-person camera's pitch angle.
    float pitchAngle { 0 };

    // The mouse's previous x-coordinate; used for mouse camera controls.
    int prevMouseX { 0 };

    // The mouse's previous y-coordinate; used for mouse camera controls.
    int prevMouseY { 0 };

    // The local character velocity based on which of the WASD keys are pressed; needs to be transformed from local space to world space.
    glm::vec3 wasdVelocity { 0 };

    // boolean that either allows or disallows mouse movement
    bool allowMouseMovement{ true };

    // The Xbox controller handle.
    ofxXboxController controller;

    // Animate shield position
    float time{ 0 };
    glm::vec3 shieldPosition{ 0, 1, 0 };

    // Reloads the shaders while the application is running.
    void reloadShaders();

    // Updates the first-person camera bsed on some 2D input (from a mouse or Xbox controller).
    //void updateFPCamera(float dx, float dy);

    // Draws the whole scene using the currently active framebuffer
    void drawScene(CameraMatrices& camMatrices, int reflection);

    // Called to update rotation of the camera from mouse movement
    void updateCameraRotation(float dx, float dy);
>>>>>>> Stashed changes
};
