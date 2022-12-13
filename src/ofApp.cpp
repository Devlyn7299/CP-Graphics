#include "ofApp.h"
#include "CameraMatrices.h"
#include "GLFW/glfw3.h"
#include "buildTerrainMesh.h"
#include "calcTangents.h"
#include "DirectionalLight.h"
#include "ofAppPlane.h"
#include "ofAppTerrain.h"

using namespace glm;
ofAppPlane ofPlane;

void buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh)
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

void ofApp::reloadShaders()
{
    directionalLightShader.load("shaders/mesh.vert", "shaders/directionalLight.frag");
    terrainShader.load("shaders/mesh.vert", "shaders/directionalLight.frag");
    waterShader.load("shaders/mesh.vert", "shaders/water.frag");
    shadowShader.load("shaders/mesh.vert", "shaders/shadow.frag");

    // Shadow stuff
    directionalLightShader2.load("shaders/mesh.vert", "shaders/directionalLight2.frag");
    //pointLightShader.load("shaders/my.vert", "shaders/pointLight.frag");
    //spotLightShader.load("shaders/my.vert", "shaders/spotLight.frag");
    planeShader.load("shaders/mesh.vert", "shaders/shadow2.frag");
    //skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");



    // Setup terrain shader uniform variables
    terrainShader.begin();
    terrainShader.setUniform3f("lightDir", normalize(vec3(1, 1, -1)));
    terrainShader.setUniform3f("lightColor", vec3(1, 1, 0.5));
    terrainShader.setUniform3f("ambientColor", vec3(0.15, 0.15, 0.3));
    terrainShader.setUniform1f("gammaInv", 1.0f / 2.2f);
    terrainShader.setUniform3f("meshColor", vec3(0.25, 0.5, 0.25));
    terrainShader.setUniformMatrix3f("normalMatrix", mat3());
    terrainShader.end();



    needsReload = false;
}

void ofApp::updateCameraRotation(float dx, float dy)
{
    using namespace glm;

    pitchAngle -= dy;
    // -89 to +89 degrees, converted to radians
    pitchAngle = clamp(pitchAngle, radians(-89.0f), radians(89.0f));

    headAngle -= dx;
}

//--------------------------------------------------------------
void ofApp::setup()
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

    // Initialize Xbox controller handle
    controller.setup();

    auto window { ofGetCurrentWindow() };

    // Uncomment the following line to let GLFW take control of the cursor so we have unlimited cursor space
    // glfwSetInputMode(dynamic_pointer_cast<ofAppGLFWWindow>(window)->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load the shaders for the first time.
    reloadShaders();

    // Load shield mesh
    shieldMesh.load("models/shield.ply");



    calcTangents(shieldMesh);

    // Load shield textures
    shieldDiffuse.load("textures/shield_diffuse.png");
    shieldNormal.load("textures/shield_normal.png");
    //shieldSpecular.load("textures/shield_spec.png");


        // building plane mesh for FBO stuff
    buildPlaneMesh(20000.0, 20000.0, 752.475, planeMesh);

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
    //fpCamera.position = vec3(0, 752.475, 0);
    // Setup the world parameters.
    world.heightmap = &heightmap.getPixels();
    world.dimensions = vec3((heightmap.getWidth() - 1), heightmapScale, heightmap.getHeight() - 1);
    world.gravity = -world.dimensions.y * config.gravity;
    world.waterHeight = config.waterHeight * world.dimensions.y;

    cout << "Downscaling heightmap for far LOD..." << endl;

    // Load a low resolution heightmap for distant land.
    ofShortImage heightmapFarLOD { heightmap };
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

    // Define character height relative to gravity
    float charHeight = -world.gravity * 0.1685f;
    //character.setCharacterHeight(charHeight);

    // Set initial character position.
    //character.setPosition(fpCamera.position);

    // Set character movement parameters
    characterWalkSpeed = 10 * charHeight; // much faster than realism for efficiently moving around the map
    //characterJumpSpeed = 10 * charHeight; // much higher than realism for efficiently moving around the map





    // Shadow stuff
    // Load shield mesh
    shieldMesh.load("models/shield.ply");
    calcTangents(shieldMesh);
    // Load shield textures
    shieldDiffuse.load("textures/shield_diffuse.png");
    shieldNormal.load("textures/shield_normal.png");
    shieldSpecular.load("textures/shield_spec.png");
    // Allocate a 1024x1024 RGB+alpha framebuffer
    ofFboSettings fboSettings{};
    fboSettings.width = 1024;
    fboSettings.height = 1024;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    fbo.allocate(fboSettings);


}

//void ofApp::updateFPCamera(float dx, float dy)
//{
//    // Turn left/right.  Use quadratic response curve.
//    float dxQuad = dx * abs(dx);
//
//    // Calculate rotation around the y-axis.
//    headAngle += dxQuad;
//    if (headAngle > 2 * pi<float>())
//    {
//        headAngle -= 2 * pi<float>();
//    }
//    else if (headAngle < 0)
//    {
//        headAngle += 2 * pi<float>();
//    }
//
//    // Look up/down.  Use quadratic response curve.
//    pitchAngle = clamp(pitchAngle + dy * abs(dy), -pi<float>() / 2, pi<float>() / 2);
//
//    // Update character's rotation: rotation around the x-axis.
//    fpCamera.rotation = rotate(headAngle, vec3(0, 1, 0)) * rotate(pitchAngle, vec3(1, 0, 0));
//}

//--------------------------------------------------------------
void ofApp::update()
{
    // Make sure that jump doesn't get spammed while the button is being held down.
    bool previousJumpState { controller.getGamepad(0).a };

    controller.update();

    Gamepad gamepad { controller.getGamepad(0) };

    mat3 headRotationMatrix { rotate(headAngle, vec3(0, 1, 0)) };

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
    
    // Advance character physics.
    //character.update(ofGetLastFrameTime());

    // Use new character position as the camera position.
    //fpCamera.position = character.getPosition();

    // Load new cells if necessary:
    cellManager.optimizeForPosition(fpCamera.position);
    //cout << fpCamera.position << endl;
    //10281.8, 764.475, 8699.67


    // Shield stuff
    time += dt;
    shieldPosition = vec3(10281.8 + sin(time), 752.475, 8699.67);

}

void drawMesh(const CameraMatrices& camMatrices,
    const DirectionalLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix = glm::mat4{})
{
    using namespace glm;

    // assumes shader is already active
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

void ofApp::drawScene(CameraMatrices& camMatrices, int reflection)
{
    using namespace glm;

    // Define the directional light
    DirectionalLight dirLight{ };
    dirLight.direction = normalize(vec3(1, 1, -1));
    dirLight.color = vec3(1, 1, 0.5);  // white light
    dirLight.intensity = 1;

    //// Define the point light
    //PointLight pointLight{ };
    //pointLight.position = vec3(0, 0, 1.5);
    //pointLight.color = vec3(1, 1, 1);  // white light
    //pointLight.intensity = 1;

    //// Define the spot light
    //SpotLight spotLight{ };
    //spotLight.position = vec3(0, 0, 1.5);
    //spotLight.direction = vec3(0, 0, -1);
    //spotLight.cutoff = cos(radians(15.0f /* degrees */));
    //spotLight.color = vec3(1, 1, 1);  // white light
    //spotLight.intensity = 1;

    directionalLightShader2.begin();

    // Set up the textures in advance
    directionalLightShader2.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    directionalLightShader2.setUniformTexture("normalTex", shieldNormal, 1); // IMPORTANT: Use a different slot
    //directionalLightShader.setUniformTexture("specularTex", shieldSpecular, 2);
    //directionalLightShader.setUniform1f("shininess", 64.0);
    //directionalLightShader.setUniform1i("reflection", reflection);
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader2, shieldMesh,
        translate(vec3(shieldPosition)));

    directionalLightShader2.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    float aspect { static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight()) };

    // Calculate an appropriate distance for a plane conceptually dividing the high level-of-detail close terrain and the lower level-of-detail distant terrain (or fog with no distant terrain).
    float midLODPlane { length(vec2(
        0.5f * NEAR_LOD_SIZE * NEAR_LOD_RANGE,
        fpCamera.position.y - world.getTerrainHeightAtPosition(fpCamera.position))) };
    cout << midLODPlane << endl;
    midLODPlane = 250;

    // Calculate an appropriate far plane for the distant terrain.
    float farPlaneDistant { 0.25f * heightmap.getWidth() };

    // Calculate view and projection matrices for the distant terrain.
    // Set the clipping plane to be 25% of the dividing plane to allow for sufficient overlap for a smooth transition.
    CameraMatrices camFarMatrices { fpCamera, aspect, midLODPlane * 0.25f, farPlaneDistant };

    // Disable depth clamping for distant terrain
    glDisable(GL_DEPTH_CLAMP);

    // Distant terrain
    terrainShader.begin();
    terrainShader.setUniform1f("startFade", farPlaneDistant * 0.95f);
    terrainShader.setUniform1f("endFade", farPlaneDistant * 1.0f);
    terrainShader.setUniformMatrix4f("modelView", camFarMatrices.getView());
    terrainShader.setUniformMatrix4f("mvp", camFarMatrices.getProj()* camFarMatrices.getView());

    // Draw the static distant terrain mesh.
    farTerrainVBO.drawElements(GL_TRIANGLES, farTerrainVBO.getNumIndices());

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
    waterShader.setUniformMatrix4f("mvp", camFarMatrices.getProj()* camFarMatrices.getView());

    // Draw the water plane.
    waterPlane.draw();

    waterShader.end();

    // Clear depth buffer as our clipping planes have changed
    glClear(GL_DEPTH_BUFFER_BIT);

    float nearPlane = -world.gravity * 0.01f; // Define near plane relative to world gravity (which is also proportional to player character height)

    // Calculate view and projection matrices for the close terrain.
    CameraMatrices camNearMatrices { fpCamera, aspect, nearPlane, midLODPlane };

    //// Debugging matrices:
    //midLODPlane = 100000; // to push back fog
    //mat4 proj { glm::perspective(radians(100.0f), aspect, 1.0f, 100000.0f) };
    //mat4 view { glm::rotate(radians(60.0f), vec3(1, 0, 0)) *
    //    glm::translate(vec3(-10240, -8000, -16000)) };
    //mat4 modelView { view };
    //mat4 mvp { proj * view };


    // Shadow stuff
    mat4 spotShadowView = (lookAt(vec3(0, 2, 1), vec3(0, 1, 0), vec3(0, 1, 0)));
    mat4 spotShadowProj = perspective((radians(90.0f)), aspect, nearPlane, midLODPlane);

    // in lookAt, use spot light position for eye, position + direction for center, and (0,1,0) for up
        //10281.8, 764.475, 8699.67

    mat4 dirShadowView = lookAt(vec3(1, 1, -1) + fpCamera.position, fpCamera.position, vec3(0, 1, 0));
    mat4 dirShadowProj = ortho(-10.0f, 10.0f, -10.0f, 10.0f, -100.0f, 100.0f);
    mat4 dirShadowProjClose = ortho(-5.0f, 5.0f, -5.0f, 5.0f, -50.0f, 50.0f);

    // FBO camera
    CameraMatrices dirFboCamMatrices{ dirShadowView, dirShadowProj };
    CameraMatrices dirFboCamMatrices2{ dirShadowView, dirShadowProjClose };
    CameraMatrices spotFboCamMatrices{ spotShadowView, spotShadowProj };
    //camNearMatrices = dirFboCamMatrices;

    fbo.begin();
    ofClear(0, 0, 0, 0);
    drawScene(dirFboCamMatrices, 0);
    //drawScene(dirFboCamMatrices2, 0);

    //drawScene(spotFboCamMatrices, 1);
    fbo.end();


    drawScene(camNearMatrices, 0);




    mat4 modelView { camNearMatrices.getView() };
    mat4 mvp { camNearMatrices.getProj() * camNearMatrices.getView() };

    // Near terrain
    //terrainShader.begin();
    //terrainShader.setUniform1f("startFade", midLODPlane * 0.75f);
    //terrainShader.setUniform1f("endFade", midLODPlane * 0.95f);
    //terrainShader.setUniformMatrix4f("modelView", modelView);
    //terrainShader.setUniformMatrix4f("mvp", mvp);




    planeShader.begin();

    planeShader.setUniform1f("startFade", midLODPlane * 0.75f);
    planeShader.setUniform1f("endFade", midLODPlane * 0.95f);
    planeShader.setUniformMatrix4f("modelView", modelView);
    planeShader.setUniformMatrix4f("mvp", mvp);
    planeShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    planeShader.setUniformTexture("normalTex", shieldNormal, 1);

    //planeShader.setUniform3f("spotLightPos", vec3(0, 2, 1));
    //planeShader.setUniform3f("spotLightConeDir", vec3(0, -1, -1));
    //planeShader.setUniform1f("spotLightCutoff", cos(radians(10.0f /* degrees */)));

    //planeShader.setUniform3f("spotLightColor", vec3(0.9, 0.9, 0.9));
    planeShader.setUniform3f("ambientColor", vec3(0.15, 0.15, 0.3));
    planeShader.setUniform1f("gammaInv", 1.0f / 2.2f);
    planeShader.setUniform3f("meshColor", vec3(0.25, 0.5, 0.25));


    //dirLight.direction = normalize(vec3(1, 1, 1));
    //dirLight.color = vec3(0.9, 0.9, 0.1);  // white light

    planeShader.setUniform3f("dirLightDir", (vec3(1, 1, -1)));
    planeShader.setUniform3f("dirLightColor", vec3(1, 1, 0.5));

    planeShader.setUniform3f("cameraPos", camNearMatrices.getCamera().position);
    planeShader.setUniformMatrix4f("mvp", camNearMatrices.getProj() * camNearMatrices.getView());
    planeShader.setUniformMatrix4f("model", mat4());
    planeShader.setUniformMatrix3f("normalMatrix", mat3());
    planeShader.setUniform1f("lightType", 1);
    planeShader.setUniformMatrix4f("shadowMatrix",
        dirFboCamMatrices.getProj() * dirFboCamMatrices.getView());
    //spotFboCamMatrices.getProj()* spotFboCamMatrices.getView());
    planeShader.setUniform1f("lightType", 2);

    planeShader.setUniformMatrix4f("shadowMatrix2",
        spotFboCamMatrices.getProj() * spotFboCamMatrices.getView());
    planeShader.setUniformTexture("fboTexture", fbo.getDepthTexture(), 0);

    // Draw the high level-of-detail cells.
    cellManager.drawActiveCells(fpCamera.position, midLODPlane);
    //planeMesh.draw();

    planeShader.end();

    //terrainShader.end();

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

void ofApp::exit()
{
    cellManager.stop();
    controller.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
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

    // Added R and F to go up and down, relative to where the camera is pointing
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
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
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
void ofApp::mouseMoved(int x, int y)
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

void ofApp::drawFrameTime()
{
    glDisable(GL_CULL_FACE);
    std::string frameTimeStr{
        to_string(ofGetLastFrameTime() * 1000.0)
            + "ms\n" + to_string(ofGetFrameRate()) + "fps"
    };
    ofDrawBitmapStringHighlight(frameTimeStr, glm::vec2(50, 50));
    //glEnable(GL_CULL_FACE);
}

