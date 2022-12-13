#pragma once

#include "ofMain.h"
#include "World.h"
#include "CellManager.h"
#include "Camera.h"
#include "CharacterPhysics.h"
#include "ofxXboxController.h"
#include "CameraMatrices.h"
#include "ofxCubemap.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"


struct HeightmapConfig2
{
public:
    std::string hiResName;
    std::string loResName;
    float scale;
    float waterHeight;
    float gravity;
};

class ofAppPlane : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    //void exit();

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


    // Reloads the shaders while the application is running.
    void reloadShaders();

    // Updates the first-person camera bsed on some 2D input (from a mouse or Xbox controller).
    //void updateFPCamera(float dx, float dy);

    // Draws the whole scene using the currently active framebuffer
    void drawScene(CameraMatrices& camMatrices, int reflection);

    // Called to update rotation of the camera from mouse movement
    void updateCameraRotation(float dx, float dy);

    void drawFrameTime();

    void drawCube(const CameraMatrices& camMatrices);

    void buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh);

    void drawMeshPoint(const CameraMatrices& camMatrices,
        const PointLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    void drawMeshSpot(const CameraMatrices& camMatrices,
        const SpotLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    void drawMeshDir(const CameraMatrices& camMatrices,
        const DirectionalLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    void drawMeshMix(const CameraMatrices& camMatrices,
        const SpotLight& spotLight,
        const DirectionalLight& dirLight,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});


private:
    // The number of quads in each row and column of a single terrain cell for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_SIZE{ 256 };

    // The number of cells beyond the current cell that the cell manager loads for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_RANGE{ 4 };

    const HeightmapConfig2 tamriel{ "TamrielBeta_10_2016_01.png", "TamrielLowRes.png", 1600.0f, 0.4375f, 0.05f };
    //const HeightmapConfig2 random { "RandomHiRes.png", "RandomLowRes.png", 800.0f, 0.5f, 0.1f };

    HeightmapConfig2 config{ tamriel };

    // The vertical scale of the heightmap.  Used to convert the pixel values in the heightmap to physical height units.
    float heightmapScale{ config.scale };

    // Non-GPU image containing the heightmap.
    ofShortImage heightmap{};

    // Plane mesh for rendering water.
    ofMesh waterPlane{};

    // Shader for rendering terrain.
    ofShader terrainShader{};

    // Shader for rendering water.
    ofShader waterShader{};

    // Shader for a directional light
    ofShader directionalLightShader;
    // Shader for a directional light
    ofShader directionalLightShader2;

    // Plane shader
    ofShader shadowShader{};

    // The main game "world" that uses the heightmap.
    World world{};

    // A cell manager for the high level-of-detail close terrain.
    CellManager<NEAR_LOD_RANGE + 1> cellManager{ heightmap, heightmapScale, NEAR_LOD_SIZE };


    // Mesh of a shield
    ofMesh shieldMesh;

    // diffuse (color) texture
    ofImage shieldDiffuse;

    // normal map
    ofImage shieldNormal;

    // specular texture
    ofImage shieldSpecular;

    // Plane shader
    //ofShader planeShader{};

    // off-screen framebuffer object
    ofFbo fbo{};

    // A single terrain mesh. Uncomment the following line if not using a cell manager.
    ofMesh farTerrain{};
    ofVbo farTerrainVBO{};

    // Set to true when the shader reload hotkey is pressed.
    bool needsReload{ true };

    // The first-person camera.
    Camera fpCamera{};

    // The helper object for physics calculations related to the first person character.
    //CharacterPhysics character { world };

    // The camera's "look" sensitivity when using the mouse.
    float camSensitivity{ 0.02f };

    // The camera's "look" sensitivity when using an Xbox controller.
    float xboxCamSensitivity{ 7.0f };

    // The character's walk speed (will be calculated based on the world's gravity).
    float characterWalkSpeed;

    // The character's jump speed (will be calculated based on the world's gravity).
    float characterJumpSpeed;

    // The first-person camera's head rotation.
    float headAngle{ 0 };

    // The first-person camera's pitch angle.
    float pitchAngle{ 0 };

    // The mouse's previous x-coordinate; used for mouse camera controls.
    int prevMouseX{ 0 };

    // The mouse's previous y-coordinate; used for mouse camera controls.
    int prevMouseY{ 0 };

    // The local character velocity based on which of the WASD keys are pressed; needs to be transformed from local space to world space.
    glm::vec3 wasdVelocity{ 0 };

    // Plane mesh
    //ofMesh planeMesh{};

    // boolean that either allows or disallows mouse movement
    bool allowMouseMovement{ true };

    // The Xbox controller handle.
    ofxXboxController controller;

    // Animate shield position
    float time{ 0 };
    glm::vec3 shieldPosition{ 0, 1, 0 };












    // Shader for a directional light
    ofShader directionalLightShader3;

    // Shader for a directional light
    ofShader pointLightShader;

    // Shader for a spot light
    ofShader spotLightShader;

    // Mesh of a shield
    ofMesh shieldMesh2;

    ofMesh staffMesh;
    ofMesh jarMesh;
    ofMesh coneMesh;
    ofMesh cube;
    ofMesh cylinderMesh;
    ofMesh sphereMesh;
    ofMesh torusMesh;

    //ofMesh swordMesh;
    //glm::vec3 swordPosition{ 0, 1, 3 };

    //ofMesh staffMesh;
    //glm::vec3 staffPosition{ 3, 0, 0 };

    //ofMesh cupMesh;
    //glm::vec3 cupPosition{ 0, 1, 0 };

    // diffuse (color) texture
    ofImage shieldDiffuse2;

    // normal map
    ofImage shieldNormal2;

    // specular texture
    ofImage shieldSpecular2;

    ofImage swordDiffuse;

    ofImage swordNormal;

    // Mesh for the skybox
    ofMesh cubeMesh;

    // Cubemap texture
    ofxCubemap cubemap;

    // Shader for rendering the skybox with a cubemap
    ofShader skyboxShader;


    // Do shaders need to be reloaded?
    // bool needsReload{ true };

    // Load the shaders for this app
    // void reloadShaders();

    // Keep track of previous mouse position
    int prevX{ 0 }, prevY{ 0 };

    // Allows us to configure how much moving the mouse affects the rotation
    float mouseSensitivity{ 0.02f };

    // Called to update rotation of the camera from mouse movement
    //void updateCameraRotation(float dx, float dy);

    // Euler transformation
    float cameraHead{ 0 };
    float cameraPitch{ 0 };

    // Camera position
    Camera camera{ glm::vec3(0, 1, 2) };

    // Camera velocity
    glm::vec3 velocity{};

    // Animate shield position
    //float time{ 0 };
    //glm::vec3 shieldPosition{ 0, 1, 0 };

    glm::vec3 staffPosition{ 0, 1, 0 };
    glm::vec3 jarPosition{ 0, 1, 0 };
    glm::vec3 conePosition{ 0, 1, 0 };
    glm::vec3 cubePosition{ 0, 1, 0 };
    glm::vec3 cylinderPosition{ -3, 1, 0 };
    glm::vec3 spherePosition{ 0, 1, 0 };
    glm::vec3 torusPosition{ 2.5, 1, -0.5 };

    bool isShieldDrawn{ true };
    bool isStaffDrawn{ true };
    bool isConeDrawn{ true };
    bool isCylinderDrawn{ true };
    bool isTorusDrawn{ true };

    //bool allowMouseMovement{ true };

    // Plane shader
    ofShader planeShader{};

    // Plane mesh
    ofMesh planeMesh{};

    // off-screen framebuffer object
    //ofFbo fbo{};

    // second fbo for second light source
    ofFbo fbo2{};

    // Draws the whole scene using the currently active framebuffer
    //void drawScene(CameraMatrices& camMatrices, int reflection);

    // Draws the FPS and frame time on the screen
    //void drawFrameTime();

    float dirLightIntensity;
    float spotLightIntensity;
    glm::vec3 spotLightDir;
    glm::vec3 spotLightPos;

    bool isPlaneExample{ true };
};
