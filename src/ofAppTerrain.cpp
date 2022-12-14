#include "ofAppTerrain.h"
#include "CameraMatrices.h"
#include "GLFW/glfw3.h"
#include "buildTerrainMesh.h"
#include "calcTangents.h"
#include "DirectionalLight.h"

using namespace glm;

void ofAppTerrain::buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh)
{
    // Northwest corner
    planeMesh.addVertex(vec3(0, height, 0));
    planeMesh.addTexCoord(vec2(0, 0));
    planeMesh.addNormal(vec3(0, 1, 0));

    // Northeast corner
    planeMesh.addVertex(vec3(0, height, depth));
    planeMesh.addTexCoord(vec2(1, 0));
    planeMesh.addNormal(vec3(0, 1, 0));

    // Southwest corner
    planeMesh.addVertex(vec3(width, height, 0));
    planeMesh.addTexCoord(vec2(0, 1));
    planeMesh.addNormal(vec3(0, 1, 0));

    // Southeast corner
    planeMesh.addVertex(vec3(width, height, depth));
    planeMesh.addTexCoord(vec2(1, 1));
    planeMesh.addNormal(vec3(0, 1, 0));

    // NW - NE - SW triangle
    planeMesh.addIndex(0);
    planeMesh.addIndex(1);
    planeMesh.addIndex(2);

    // SW - NE - SE triangle
    planeMesh.addIndex(2);
    planeMesh.addIndex(1);
    planeMesh.addIndex(3);
}

void ofAppTerrain::reloadShaders()
{
    if (needsReload)
    {
        terrainShader.load("shaders/my.vert", "shaders/lights.frag");
        waterShader.load("shaders/my.vert", "shaders/water.frag");

        lightShader.load("shaders/my.vert", "shaders/lights.frag");
        shadowShader.load("shaders/my.vert", "shaders/shadowPlane.frag");

        // Setup terrain shader uniform variables
        terrainShader.begin();
        terrainShader.setUniform1f("usedForTerrain", 1);
        terrainShader.setUniform3f("dirLightDir", normalize(vec3(1, 1, -1)));
        terrainShader.setUniform3f("dirLightColor", vec3(1, 1, 0.5));
        terrainShader.setUniform3f("ambientColor", vec3(0.15, 0.15, 0.3));
        terrainShader.setUniform1f("gammaInv", 1.0f / 2.2f);
        terrainShader.setUniform3f("meshColor", vec3(0.25, 0.5, 0.25));
        terrainShader.setUniformMatrix3f("normalMatrix", mat3());
        terrainShader.setUniform1f("usedForTerrain", 0);

        terrainShader.end();

        needsReload = false;
    }
}

void ofAppTerrain::updateCameraRotation(float dx, float dy)
{
    using namespace glm;

    pitchAngle -= dy;
    // -89 to +89 degrees, converted to radians
    pitchAngle = clamp(pitchAngle, radians(-89.0f), radians(89.0f));

    headAngle -= dx;
}

//--------------------------------------------------------------
void ofAppTerrain::setup()
{
    using namespace glm;

    ofSetVerticalSync(false);


    // Disable legacy "ArbTex"
    ofDisableArbTex();

    // Enable depth test, alpha blending, and face culling.
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    //glEnable(GL_CULL_FACE);

    // Set sky color.
    ofSetBackgroundColor(186, 186, 255);


    auto window{ ofGetCurrentWindow() };

    // Uncomment the following line to let GLFW take control of the cursor so we have unlimited cursor space
    // glfwSetInputMode(dynamic_pointer_cast<ofAppGLFWWindow>(window)->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load the shaders for the first time.
    reloadShaders();

    // Initialize the camera.
    headAngle = radians(180.0f);
    fpCamera.fov = radians(90.0f);
    fpCamera.rotation = rotate(headAngle, vec3(0, 1, 0)); // Rotate 180 degrees around the y-axis.

    cout << "Loading heightmap..." << endl;

    // Load heightmap
    heightmap.setUseTexture(false);
    heightmap.load(config.hiResName);
    assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

    // Set initial camera position.
    fpCamera.position = vec3((heightmap.getWidth() - 1) * 0.5f, 850, (heightmap.getHeight() - 1) * 0.5f);
    // Setup the world parameters.
    world.dimensions = vec3((heightmap.getWidth() - 1), heightmapScale, heightmap.getHeight() - 1);
    world.gravity = -world.dimensions.y * config.gravity;
    world.waterHeight = config.waterHeight * world.dimensions.y;

    cout << "Downscaling heightmap for far LOD..." << endl;

    // Load a low resolution heightmap for distant land.
    ofShortImage heightmapFarLOD{ heightmap };
    heightmapFarLOD.setUseTexture(false);
    heightmapFarLOD.load(config.loResName);

    // Establish scale between high-res and low-res.
    float farLODScale{ (heightmap.getHeight() - 1) / (heightmapFarLOD.getHeight() - 1) };

    // Build a single distant terrain mesh.
    cout << "Building distant terrain..." << endl;
    buildTerrainMesh(farTerrain, heightmapFarLOD, 0, 0, heightmapFarLOD.getWidth() - 1, heightmapFarLOD.getHeight() - 1, vec3(farLODScale, heightmapScale, farLODScale));
    farTerrainVBO.setMesh(farTerrain, GL_STATIC_DRAW);

    cout << "Building high-res terrain meshes..." << endl;
    cellManager.initializeForPosition(fpCamera.position);

    cout << "DONE!" << endl;

    // Create the water plane
    buildPlaneMesh(heightmap.getWidth() - 1, heightmap.getHeight() - 1, world.waterHeight, waterPlane);

    // Load meshes
    bigStaffMesh.load("models/bigStaff.ply");
    bigJarMesh.load("models/bigJar.ply");
    calcTangents(bigStaffMesh);
    calcTangents(bigJarMesh);

    // Allocate a 1024x1024 RGB+alpha framebuffer
    ofFboSettings fboSettings{};
    fboSettings.width = 1024;
    fboSettings.height = 1024;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    fbo.allocate(fboSettings);
}

//--------------------------------------------------------------
void ofAppTerrain::update()
{
    mat3 headRotationMatrix{ rotate(headAngle, vec3(0, 1, 0)) };

    float dt{ static_cast<float>(ofGetLastFrameTime()) };

    // Update position using velocity and dt.
    using namespace glm;
    fpCamera.position += mat3(rotate(headAngle, vec3(0, 1, 0)))
        * wasdVelocity * dt;
    fpCamera.rotation = rotate(headAngle, vec3(0, 1, 0)) *
        rotate(pitchAngle, vec3(1, 0, 0));

    if (needsReload)
    {
        // Reload shaders if the hotkey was pressed.
        reloadShaders();
    }

    // Load new cells if necessary:
    cellManager.optimizeForPosition(fpCamera.position);

    // Move meshes
    time += dt;
    bigStaffPosition = vec3(10281.8 + 4 * sin(time), 752.475, 8699.67);
    bigJarPosition = vec3(10291.8, 752.475, 8699.67 - 4 * sin(time));
}

void ofAppTerrain::drawMesh(const CameraMatrices& camMatrices,
    const DirectionalLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
{
    using namespace glm;

    // assumes shader is already active
    shader.setUniform3f("dirLightDir", normalize(light.direction));
    shader.setUniform3f("dirLightColor", light.color * light.intensity);
    shader.setUniform3f("cameraPosition", camMatrices.getCamera().position);
    shader.setUniform3f("lightDir", light.direction);
    shader.setUniform3f("lightColor", light.color * light.intensity);
    shader.setUniform3f("ambientColor", ambientLight);
    shader.setUniformMatrix4f("mvp",
        camMatrices.getProj() * camMatrices.getView() * modelMatrix);
    shader.setUniformMatrix3f("normalMatrix",
        inverse(transpose(mat3(modelMatrix))));
    shader.setUniformMatrix4f("model", modelMatrix);
    mesh.draw();
}

void ofAppTerrain::drawScene(CameraMatrices& camMatrices, int reflection)
{
    using namespace glm;

    // Define the directional light
    DirectionalLight dirLight{ };
    dirLight.direction = normalize(vec3(1, 1, -1));
    dirLight.color = vec3(1, 1, 0.5);  // white light
    dirLight.intensity = 1;

    lightShader.begin();

    // Set up the textures in advance
    lightShader.setUniform1f("hasTexture", 0);

    if (isStaffDrawn)
    drawMesh(camMatrices, dirLight, vec3(0.1),
        lightShader, bigStaffMesh,
        translate(vec3(bigStaffPosition)));
    if (isJarDrawn)
    drawMesh(camMatrices, dirLight, vec3(0.1),
        lightShader, bigJarMesh,
        translate(vec3(bigJarPosition)));

    lightShader.end();
}

//--------------------------------------------------------------
void ofAppTerrain::draw()
{
    float aspect{ static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight()) };

    // Calculate an appropriate distance for a plane conceptually dividing the high level-of-detail close terrain and the lower level-of-detail distant terrain (or fog with no distant terrain).
    float midLODPlane{ length(vec2(
        0.5f * NEAR_LOD_SIZE * NEAR_LOD_RANGE,
        fpCamera.position.y - world.getTerrainHeightAtPosition(fpCamera.position))) };
    //cout << midLODPlane << endl;
    midLODPlane = 250;

    // Calculate an appropriate far plane for the distant terrain.
    float farPlaneDistant{ 0.25f * heightmap.getWidth() };

    // Calculate view and projection matrices for the distant terrain.
    // Set the clipping plane to be 25% of the dividing plane to allow for sufficient overlap for a smooth transition.
    CameraMatrices camFarMatrices{ fpCamera, aspect, midLODPlane * 0.25f, farPlaneDistant };

    // Disable depth clamping for distant terrain
    glDisable(GL_DEPTH_CLAMP);

    // Distant terrain
    terrainShader.begin();
    terrainShader.setUniform1f("usedForTerrain", 1);
    terrainShader.setUniform1f("startFade", farPlaneDistant * 0.95f);
    terrainShader.setUniform1f("endFade", farPlaneDistant * 1.0f);
    terrainShader.setUniformMatrix4f("modelView", camFarMatrices.getView());
    terrainShader.setUniformMatrix4f("mvp", camFarMatrices.getProj() * camFarMatrices.getView());

    // Draw the static distant terrain mesh.
    farTerrainVBO.drawElements(GL_TRIANGLES, farTerrainVBO.getNumIndices());
    terrainShader.setUniform1f("usedForTerrain", 0);

    terrainShader.end();

    // Enable depth clamping for water to cover up distant terrain regardless of depth values.
    // It seems that depth clamping can be left on for near terrain without any undesired effects.
    glEnable(GL_DEPTH_CLAMP);

    // Distant water
    waterShader.begin();
    waterShader.setUniform3f("meshColor", vec3(0.64, 0.73, 0.81));
    waterShader.setUniform1f("startFade", farPlaneDistant * 0.95f);
    waterShader.setUniform1f("endFade", farPlaneDistant * 1.0f);
    waterShader.setUniformMatrix4f("modelView", camFarMatrices.getView());
    waterShader.setUniformMatrix4f("mvp", camFarMatrices.getProj() * camFarMatrices.getView());

    // Draw the water plane.
    waterPlane.draw();

    waterShader.end();

    // Clear depth buffer as our clipping planes have changed
    glClear(GL_DEPTH_BUFFER_BIT);

    float nearPlane = -world.gravity * 0.01f; // Define near plane relative to world gravity (which is also proportional to player character height)

    // Calculate view and projection matrices for the close terrain.
    CameraMatrices camNearMatrices{ fpCamera, aspect, nearPlane, midLODPlane };

    // in lookAt, use spot light position for eye, position + direction for center, and (0,1,0) for up
    mat4 dirShadowView = lookAt(vec3(1, 1, -1) + fpCamera.position, fpCamera.position, vec3(0, 1, 0));
    mat4 dirShadowProj = ortho(-50.0f, 50.0f, -50.0f, 50.0f, -500.0f, 500.0f);

    // FBO camera
    CameraMatrices dirFboCamMatrices{ dirShadowView, dirShadowProj };

    fbo.begin();
    ofClear(0, 0, 0, 0);
    drawScene(dirFboCamMatrices, 0);
    fbo.end();

    drawScene(camNearMatrices, 0);

    mat4 modelView{ camNearMatrices.getView() };
    mat4 mvp{ camNearMatrices.getProj() * camNearMatrices.getView() };

    shadowShader.begin();

    shadowShader.setUniform1f("usedForTerrain", 1);

    shadowShader.setUniform1f("startFade", midLODPlane * 0.75f);
    shadowShader.setUniform1f("endFade", midLODPlane * 0.95f);
    shadowShader.setUniformMatrix4f("modelView", modelView);
    shadowShader.setUniformMatrix4f("mvp", mvp);

    shadowShader.setUniform3f("ambientColor", vec3(0.15, 0.15, 0.3));
    shadowShader.setUniform1f("gammaInv", 1.0f / 2.2f);
    shadowShader.setUniform3f("meshColor", vec3(0.25, 0.5, 0.25));


    shadowShader.setUniform3f("dirLightDir", (vec3(1, 1, -1)));
    shadowShader.setUniform3f("dirLightColor", vec3(1, 1, 0.5));

    shadowShader.setUniform3f("cameraPos", camNearMatrices.getCamera().position);
    shadowShader.setUniformMatrix4f("mvp", camNearMatrices.getProj() * camNearMatrices.getView());
    shadowShader.setUniformMatrix4f("model", mat4());
    shadowShader.setUniformMatrix3f("normalMatrix", mat3());
    shadowShader.setUniformMatrix4f("shadowMatrix",
        dirFboCamMatrices.getProj() * dirFboCamMatrices.getView());

    shadowShader.setUniformTexture("fboTexture", fbo.getDepthTexture(), 0);

    // Draw the high level-of-detail cells.
    cellManager.drawActiveCells(fpCamera.position, midLODPlane);

    shadowShader.end();

    // Near water
    waterShader.begin();
    waterShader.setUniform3f("meshColor", vec3(0.64, 0.73, 0.81));
    waterShader.setUniform1f("startFade", midLODPlane * 0.95f);
    waterShader.setUniform1f("endFade", midLODPlane);
    waterShader.setUniformMatrix4f("modelView", modelView);
    waterShader.setUniformMatrix4f("mvp", mvp);

    // Draw the water plane.
    waterPlane.draw();

    waterShader.end();
    drawFrameTime();

}

void ofAppTerrain::exit()
{
    cellManager.stop();
}

//--------------------------------------------------------------
void ofAppTerrain::keyPressed(int key)
{
    if (key == 'w')
    {
        wasdVelocity.z = -50;
    }
    else if (key == 's')
    {
        wasdVelocity.z = 50;
    }
    else if (key == 'a')
    {
        wasdVelocity.x = -50;
    }
    else if (key == 'd')
    {
        wasdVelocity.x = 50;
    }

    // Added R and F to go up and down
    else if (key == 'f')
    {
        wasdVelocity.y = -50;
    }
    else if (key == 'r')
    {
        wasdVelocity.y = 50;
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

    if (key == '1')
    {
        isStaffDrawn = !isStaffDrawn;
    }

    if (key == '2')
    {
        isJarDrawn = !isJarDrawn;
    }

}

//--------------------------------------------------------------
void ofAppTerrain::keyReleased(int key)
{
    if (key == 'w' || key == 's')
    {
        wasdVelocity.z = 0;
    }
    else if (key == 'a' || key == 'd')
    {
        wasdVelocity.x = 0;
    }
    else if (key == 'f' || key == 'r')
    {
        wasdVelocity.y = 0;
    }
}

//--------------------------------------------------------------
void ofAppTerrain::mouseMoved(int x, int y)
{
    if (allowMouseMovement) {
        if (prevMouseX != 0 && prevMouseY != 0)
        {
            // Previous mouse position has been initialized.
            // Calculate dx and dy
            int dx = x - prevMouseX;
            int dy = y - prevMouseY;

            updateCameraRotation(camSensitivity * dx, camSensitivity * dy);
        }
    }
    prevMouseX = x;
    prevMouseY = y;
}

//--------------------------------------------------------------
void ofAppTerrain::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofAppTerrain::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofAppTerrain::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofAppTerrain::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofAppTerrain::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofAppTerrain::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofAppTerrain::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofAppTerrain::dragEvent(ofDragInfo dragInfo)
{

}

void ofAppTerrain::drawFrameTime()
{
    glDisable(GL_CULL_FACE);
    std::string frameTimeStr{
        to_string(ofGetLastFrameTime() * 1000.0)
            + "ms\n" + to_string(ofGetFrameRate()) + "fps"
    };
    ofDrawBitmapStringHighlight(frameTimeStr, glm::vec2(50, 50));
    //glEnable(GL_CULL_FACE);
}

