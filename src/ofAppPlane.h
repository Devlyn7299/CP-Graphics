#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "CameraMatrices.h"
#include "ofxCubemap.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

class ofAppPlane : public ofBaseApp
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
    // SHADER STUFF
    
    // Shader for a directional light
    ofShader directionalLightShader;

    // Shader for a point light
    ofShader pointLightShader;

    // Shader for a spot light
    ofShader spotLightShader;

    // Variables for spot light and directional light values to be easily changed
    float dirLightIntensity{ 1 };
    float spotLightIntensity{ 1 };
    glm::vec3 spotLightDir{};
    glm::vec3 spotLightPos{};

    // Plane shader
    ofShader planeShader{};

    // Do shaders need to be reloaded?
    bool needsReload{ true };

    // Load the shaders for this app
    void reloadShaders();





    // MESH STUFF

    // Primary meshes to be used
    ofMesh shieldMesh;
    ofMesh staffMesh;
    ofMesh coneMesh;
    ofMesh cylinderMesh;
    ofMesh torusMesh;

    // Positions of meshes, to be changed if they get animated
    float time{ 0 };
    glm::vec3 shieldPosition{ 0, 1, 0 };
    glm::vec3 staffPosition{ 0, 1, 0 };
    glm::vec3 conePosition{ 0, 1, 0 };
    glm::vec3 cylinderPosition{ -3, 1, 0 };
    glm::vec3 torusPosition{ 2.5, 1, -0.5 };

    // Booleans to determine whether a mesh is currently being drawn
    bool isShieldDrawn{ true };
    bool isStaffDrawn{ true };
    bool isConeDrawn{ true };
    bool isCylinderDrawn{ true };
    bool isTorusDrawn{ true };

    // Shield diffuse, normal, and specular maps
    ofImage shieldDiffuse;
    ofImage shieldNormal;
    ofImage shieldSpecular;

    // Draws a mesh that is affected by a point light
    void drawMeshPoint(const CameraMatrices& camMatrices,
        const PointLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    // Draws a mesh that is affected by a spot light
    void drawMeshSpot(const CameraMatrices& camMatrices,
        const SpotLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    // Draws a mesh that is affected by a directional light
    void drawMeshDir(const CameraMatrices& camMatrices,
        const DirectionalLight& light,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});

    // Draws a mesh that is affected by multiple light sources
    void drawMeshMix(const CameraMatrices& camMatrices,
        const SpotLight& spotLight,
        const DirectionalLight& dirLight,
        const glm::vec3 ambientLight,
        ofShader& shader, ofMesh& mesh,
        glm::mat4 modelMatrix = glm::mat4{});





    // SKYMAP STUFF

    // Shader for rendering the skybox with a cubemap
    ofShader skyboxShader;

    // Mesh for the skybox
    ofMesh cubeMesh;

    // Cubemap texture
    ofxCubemap cubemap;

    // Function to draw the cubemap
    void drawCube(const CameraMatrices& camMatrices);





    // CAMERA STUFF
    
    // Camera position
    Camera camera{ glm::vec3(0, 1, 2) };

    // Camera velocity
    glm::vec3 velocity{};

    // Allows or disallows mouse movement
    bool allowMouseMovement{ true };

    // Keep track of previous mouse position
    int prevX{ 0 }, prevY{ 0 };

    // Allows us to configure how much moving the mouse affects the rotation
    float mouseSensitivity{ 0.02f };

    // Euler transformation
    float cameraHead{ 0 };
    float cameraPitch{ 0 };

    // Called to update rotation of the camera from mouse movement
    void updateCameraRotation(float dx, float dy);





    // PLANE STUFF

     // Plane mesh
    ofMesh planeMesh{};

    // off-screen framebuffer object for directional shadows
    ofFbo fbo{};

    // second fbo for spot light shadows
    ofFbo fbo2{};

    // Builds a plane mesh based on width, depth, height, and what mesh to draw
    void buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh);





    // EXTRA FUNCTIONS

    // Draws the whole scene using the currently active framebuffer
    void drawScene(CameraMatrices& camMatrices, int reflection);

    // Draws the FPS and frame time on the screen
    void drawFrameTime();

};
