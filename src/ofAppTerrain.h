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


struct HeightmapConfig3
{
public:
    std::string hiResName;
    std::string loResName;
    float scale;
    float waterHeight;
    float gravity;
};

class ofAppTerrain : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();

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
    // The number of quads in each row and column of a single terrain cell for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_SIZE{ 256 };

    // The number of cells beyond the current cell that the cell manager loads for the close, high level-of-detail terrain.
    const static unsigned int NEAR_LOD_RANGE{ 4 };

    const HeightmapConfig3 tamriel{ "TamrielBeta_10_2016_01.png", "TamrielLowRes.png", 1600.0f, 0.4375f, 0.05f };

    HeightmapConfig3 config{ tamriel };

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

    ofMesh bigStaffMesh;
    ofMesh bigJarMesh;

    // Mesh of a shield
    ofMesh shieldMesh;

    // diffuse (color) texture
    ofImage shieldDiffuse;

    // normal map
    ofImage shieldNormal;

    // specular texture
    ofImage shieldSpecular;

    // off-screen framebuffer object
    ofFbo fbo{};

    // A single terrain mesh. Uncomment the following line if not using a cell manager.
    ofMesh farTerrain{};
    ofVbo farTerrainVBO{};

    // Set to true when the shader reload hotkey is pressed.
    bool needsReload{ true };

    // The first-person camera.
    Camera fpCamera{};

    // The camera's "look" sensitivity when using the mouse.
    float camSensitivity{ 0.02f };

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

    // boolean that either allows or disallows mouse movement
    bool allowMouseMovement{ true };

    // Animate shield position
    float time{ 0 };
    glm::vec3 shieldPosition{ 0, 1, 0 };
    glm::vec3 bigStaffPosition{ 0, 1, 0 };
    glm::vec3 bigJarPosition{ 0, 1, 0 };

    // Reloads the shaders while the application is running.
    void reloadShaders();

    // Draws the whole scene using the currently active framebuffer
    void drawScene(CameraMatrices& camMatrices, int reflection);

    // Called to update rotation of the camera from mouse movement
    void updateCameraRotation(float dx, float dy);

    void drawFrameTime();

    // Plane shader
    ofShader planeShader{};

    void buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh);

    void drawMesh(const CameraMatrices& camMatrices,
        const DirectionalLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});


};
