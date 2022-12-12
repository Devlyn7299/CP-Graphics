#include "ofApp.h"
<<<<<<< Updated upstream
#include "buildTerrainMesh.h"
=======
#include "CameraMatrices.h"
#include "GLFW/glfw3.h"
#include "buildTerrainMesh.h"
#include "calcTangents.h"
#include "DirectionalLight.h"

using namespace glm;
>>>>>>> Stashed changes

void buildCube(ofMesh& mesh)
{
<<<<<<< Updated upstream
    using namespace glm;

    // Generate the cube mesh
    mesh.addVertex(vec3(-1, -1, -1)); // front left bottom
    mesh.addVertex(vec3(-1, -1, 1)); // back  left bottom
    mesh.addVertex(vec3(-1, 1, -1)); // front left top
    mesh.addVertex(vec3(-1, 1, 1));  // back  left top
    mesh.addVertex(vec3(1, -1, -1)); // front right bottom
    mesh.addVertex(vec3(1, -1, 1));  // back  right bottom
    mesh.addVertex(vec3(1, 1, -1));  // front right top
    mesh.addVertex(vec3(1, 1, 1));   // back  right top

    ofIndexType indices[36] =
    {
        2, 3, 6, 7, 6, 3, // top
        0, 4, 1, 1, 4, 5, // bottom
        0, 1, 2, 2, 1, 3, // left
        4, 6, 5, 5, 6, 7, // right
        0, 2, 4, 4, 2, 6, // front
        1, 5, 3, 3, 5, 7, // back
    };

    mesh.addIndices(indices, 36);

    mesh.flatNormals();

    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
    {
        mesh.setNormal(i, -mesh.getNormal(i));
    }
}

void buildCircle(ofMesh& mesh, int subdiv)
{
    using namespace glm;

    // Populate with vertex positions.

    // Center
    mesh.addVertex(vec3(0));

    // Outer ring
    for (int i{ 0 }; i < subdiv; i++)
    {
        double theta{ i * (2 * PI) / subdiv };
        mesh.addVertex(vec3(cos(theta), sin(theta), 0));
    }

    for (int i{ 0 }; i < subdiv; i++)
    {
        mesh.addIndex(0);
        mesh.addIndex(1 /* outer ring start */ + i);
        mesh.addIndex(1 /* outer ring start */ + (i + 1) % subdiv);
    }

    mesh.flatNormals();

    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
    {
        mesh.setNormal(i, -mesh.getNormal(i));
    }
}

void buildCylinder(ofMesh& mesh, int subdiv)
{
    using namespace glm;

    // Populate with vertex positions.

    // Top center
    mesh.addVertex(vec3(0, 1, 0)); // vertex 0

    // Top outer ring -- vertices 1 through subdiv
    for (int i{ 0 }; i < subdiv; i++)
    {
        double theta{ i * (2 * PI) / subdiv };
        mesh.addVertex(vec3(cos(theta), 1, -sin(theta)));
    }

    // Bottom center
    mesh.addVertex(vec3(0, -1, 0)); // vertex subdiv + 1

    // Bottom outer ring -- vertices subdiv + 2 through 2 * subdiv + 1
    for (int i{ 0 }; i < subdiv; i++)
    {
        double theta{ i * (2 * PI) / subdiv };
        mesh.addVertex(vec3(cos(theta), -1, -sin(theta)));
    }

    // Start of index buffer: populate to form faces

    // Top circle
    int topRingStart{ 1 };
    for (int i{ 0 }; i < subdiv; i++)
    {
        mesh.addIndex(0); // top center
        mesh.addIndex(topRingStart + i);
        mesh.addIndex(topRingStart + (i + 1) % subdiv);
    }

    // Bottom circle
    int bottomRingStart{ subdiv + 2 };
    for (int i{ 0 }; i < subdiv; i++)
    {
        mesh.addIndex(subdiv + 1); // bottom center

        // Swap order from top circle so that the circle faces downwards instead of upwards.
        mesh.addIndex(bottomRingStart + (i + 1) % subdiv);
        mesh.addIndex(bottomRingStart + i);
    }

    // Sides
    for (int i{ 0 }; i < subdiv; i++)
    {
        // Define the indices of a single quad
        int indices[4]
        {
            topRingStart + i,
            topRingStart + (i + 1) % subdiv,
            bottomRingStart + i,
            bottomRingStart + (i + 1) % subdiv
        };

        // build the quad
        mesh.addIndex(indices[0]);
        mesh.addIndex(indices[2]);
        mesh.addIndex(indices[1]);
        mesh.addIndex(indices[1]);
        mesh.addIndex(indices[2]);
        mesh.addIndex(indices[3]);
    }

    mesh.flatNormals();

    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
    {
        mesh.setNormal(i, -mesh.getNormal(i));
    }
}

void buildSphere(ofMesh& mesh, int subdivTheta, int subdivPhi)
{
    using namespace glm;

    // Populate with vertex positions.

    // Top center (north pole)
    mesh.addVertex(vec3(0, 1, 0)); // vertex 0

    // Rings
    // Start at i = 1 since i=0 is the north pole
    for (int i{ 1 }; i < subdivTheta; i++)
    {
        double theta{ i * PI / subdivTheta };

        for (int j{ 0 }; j < subdivPhi; j++)
        {
            double phi{ j * (2 * PI) / subdivPhi };

            mesh.addVertex(vec3(sin(theta) * cos(phi), cos(theta), -sin(theta) * sin(phi)));
        }
    }

    // Bottom center (south pole)
    mesh.addVertex(vec3(0, -1, 0)); // vertex ???


    // Start of index buffer: populate to form faces

    // Top circle (around north pole)
    for (int i{ 0 }; i < subdivPhi; i++)
    {
        int topRingStart{ 1 };

        mesh.addIndex(0); // top center / north pole
        mesh.addIndex(topRingStart + i);
        mesh.addIndex(topRingStart + (i + 1) % subdivPhi);
    }

    int southPole{ (subdivTheta - 1) * subdivPhi + 1 };

    // Bottom circle (around south pole)
    for (int i{ 0 }; i < subdivPhi; i++)
    {
        int bottomRingStart{ southPole - subdivPhi };

        mesh.addIndex(southPole); // bottom center / south pole

        // Swap order from top circle so that the circle faces downwards instead of upwards.
        mesh.addIndex(bottomRingStart + (i + 1) % subdivPhi);
        mesh.addIndex(bottomRingStart + i);
    }

    // Sides
    for (int i{ 0 }; i < subdivTheta - 2; i++)
    {
        for (int j{ 0 }; j < subdivPhi; j++)
        {
            int topRingStart{ 1 + subdivPhi * i };
            int bottomRingStart{ 1 + subdivPhi * (i + 1) };

            // Define the indices of a single quad
            int indices[4]
            {
                topRingStart + j,
                topRingStart + (j + 1) % subdivPhi,
                bottomRingStart + j,
                bottomRingStart + (j + 1) % subdivPhi
            };

            // build the quad
            mesh.addIndex(indices[0]);
            mesh.addIndex(indices[2]);
            mesh.addIndex(indices[1]);
            mesh.addIndex(indices[1]);
            mesh.addIndex(indices[2]);
            mesh.addIndex(indices[3]);
        }
    }

    mesh.flatNormals();

    for (size_t i{ 0 }; i < mesh.getNumNormals(); i++)
    {
        mesh.setNormal(i, -mesh.getNormal(i));
    }
}

//--------------------------------------------------------------
void ofApp::setup()
=======
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

    // Disable legacy "ArbTex"
    ofDisableArbTex();

    // Enable depth test, alpha blending, and face culling.
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    glEnable(GL_CULL_FACE);

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
    shieldSpecular.load("textures/shield_spec.png");




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
}

void drawMesh(const CameraMatrices& camMatrices,
    const DirectionalLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix = glm::mat4{})
>>>>>>> Stashed changes
{
    ofDisableArbTex(); // IMPORTANT!

    //ofEnableDepthTest();

    glEnable(GL_CULL_FACE); // Enable face culling to test winding order

    // Load shaders for the first time
    reloadShaders();

    // Load torus mesh
    torusMesh.load("torus.ply");

    ofShortImage heightmap{};
    heightmap.setUseTexture(false);
    // 641 x 513
    heightmap.load("TamrielLowRes.png");
    assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);
    //cout << heightmap.getColor(320, 200) << endl;

<<<<<<< Updated upstream
    // Build terrain mesh
    buildTerrainMesh( terrainMesh, heightmap, 0, 0,
    heightmap.getWidth() - 1, heightmap.getHeight() - 1,
    glm::vec3(1, 50, 1));
    
    // Generate cube mesh
    buildCube(cubeMesh);

    // Generate circle mesh
    buildCircle(circleMesh, 40);

    // Generate cylinder mesh
    buildCylinder(cylinderMesh, 8);

    buildSphere(sphereMesh, 4, 8);
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
=======
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
    dirLight.direction = normalize(vec3(1, 1, 1));
    dirLight.color = vec3(1, 1, 0);  // white light
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

    directionalLightShader.begin();

    // Set up the textures in advance
    directionalLightShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    directionalLightShader.setUniformTexture("normalTex", shieldNormal, 1); // IMPORTANT: Use a different slot
    directionalLightShader.setUniformTexture("specularTex", shieldSpecular, 2);
    directionalLightShader.setUniform1f("shininess", 64.0);
    directionalLightShader.setUniform1i("reflection", reflection);
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, shieldMesh,
        translate(vec3(shieldPosition)));

    directionalLightShader.end();
>>>>>>> Stashed changes
}

//--------------------------------------------------------------
void ofApp::update()
{
<<<<<<< Updated upstream
    if (needsReload)
    {
        reloadShaders();
    }

    using namespace glm;

    // calculate world space velocity
    vec3 velocityWorldSpace{ mat3(rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0))) * velocity};

    // time since last frame
    float dt{ static_cast<float>(ofGetLastFrameTime()) };

    // update position
    position += velocityWorldSpace * dt;

    //cout << position << endl;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    using namespace glm;

    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect{ width / height };

    mat4 model{ rotate(radians(0.0f), vec3(1, 1, 1)) * scale(vec3(0.5, 0.5, 0.5)) };
    mat4 view{ (rotate(cameraPitch, vec3(1, 0, 0)) * rotate(cameraHead, vec3(0, 1, 0))) * translate(-position) };
    mat4 proj{ perspective(radians(90.0f), aspect, 0.01f, 20.0f) };

    mat4 mvp{ proj * view * model };

    shader.begin(); // start using the shader
    shader.setUniformMatrix4f("mvp", mvp);
    //cubeMesh.draw();
    terrainMesh.draw();
    shader.end(); // done with the shader
=======
    float aspect { static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight()) };

    // Calculate an appropriate distance for a plane conceptually dividing the high level-of-detail close terrain and the lower level-of-detail distant terrain (or fog with no distant terrain).
    float midLODPlane { length(vec2(
        0.5f * NEAR_LOD_SIZE * NEAR_LOD_RANGE,
        fpCamera.position.y - world.getTerrainHeightAtPosition(fpCamera.position))) };

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

    mat4 modelView { camNearMatrices.getView() };
    mat4 mvp { camNearMatrices.getProj() * camNearMatrices.getView() };

    // Near terrain
    terrainShader.begin();
    terrainShader.setUniform1f("startFade", midLODPlane * 0.75f);
    terrainShader.setUniform1f("endFade", midLODPlane * 0.95f);
    terrainShader.setUniformMatrix4f("modelView", modelView);
    terrainShader.setUniformMatrix4f("mvp", mvp);

    // Draw the high level-of-detail cells.
    cellManager.drawActiveCells(fpCamera.position, midLODPlane);

    terrainShader.end();

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
}

void ofApp::exit()
{
    cellManager.stop();
    controller.exit();
>>>>>>> Stashed changes
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'w')
    {
<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
    }

    // Added R and F to go up and down, relative to where the camera is pointing
    else if (key == 'f')
    {
        velocity.y = -1;
    }
    else if (key == 'r')
    {
<<<<<<< Updated upstream
        velocity.y = 1;
=======
        wasdVelocity.y = 50;
>>>>>>> Stashed changes
    }

    // Allowing the user to disable mouseMovement with E
    if (key == 'e')
    {
        allowMouseMovement = !allowMouseMovement;
    }

<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
<<<<<<< Updated upstream
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
=======
    if (prevMouseX != 0 && prevMouseY != 0)
    {
        // Previous mouse position has been initialized.
        // Calculate dx and dy
        int dx = x - prevMouseX;
        int dy = y - prevMouseY;

        updateCameraRotation(camSensitivity * dx, camSensitivity * dy);
    }

    prevMouseX = x;
    prevMouseY = y;
>>>>>>> Stashed changes
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
