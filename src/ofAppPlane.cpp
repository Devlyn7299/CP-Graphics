#include "ofAppPlane.h"
#include "CameraMatrices.h"
#include "calcTangents.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

void ofAppPlane::buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh)
{
    using namespace glm;


    // Northwest corner
    planeMesh.addVertex(vec3(-width, height, -depth));
    planeMesh.addTexCoord(vec2(0, 0));
    planeMesh.addNormal(vec3(0, 1, 0));

    // Northeast corner
    planeMesh.addVertex(vec3(-width, height, depth));
    planeMesh.addTexCoord(vec2(1, 0));
    planeMesh.addNormal(vec3(0, 1, 0));

    // Southwest corner
    planeMesh.addVertex(vec3(width, height, -depth));
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

//--------------------------------------------------------------
void ofAppPlane::setup()
{
    using namespace glm;

    ofSetVerticalSync(false);

    ofDisableArbTex(); 

    // Enable interpolation across sides of the cubemap
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Disable Windows cursor
    auto window{ ofGetCurrentWindow() };

    ofEnableDepthTest();

    // Load shaders for the first time
    reloadShaders();

    // Load meshes
    shieldMesh.load("models/shield.ply");
    staffMesh.load("models/staff.ply");
    coneMesh.load("models/cone.ply");
    cylinderMesh.load("models/cylinder.ply");
    torusMesh.load("models/torus.ply");

    calcTangents(shieldMesh);

    // Load shield textures
    shieldDiffuse.load("textures/shield_diffuse.png");
    shieldNormal.load("textures/shield_normal.png");

    // Load skybox mesh
    cubeMesh.load("models/cube.ply");

    // Load cubemap images
    cubemap.load("textures/cube_front.jpg", "textures/cube_back.jpg",
        "textures/cube_right.jpg", "textures/cube_left.jpg",
        "textures/cube_top.jpg", "textures/cube_bottom.jpg");

    // Generate mipmap for irradiance calculation
    cubemap.getTexture().generateMipmap();

    // building plane mesh for FBO stuff
    buildPlaneMesh(10.0, 10.0, 0.0, planeMesh);

    // Allocate a 1024x1024 RGB+alpha framebuffer
    ofFboSettings fboSettings{};
    fboSettings.width = 1024;
    fboSettings.height = 1024;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    fbo.allocate(fboSettings);
    fbo2.allocate(fboSettings);
}

void ofAppPlane::reloadShaders()
{
    // Reloads all shaders
    if (needsReload) {
        lightShader.load("shaders/my.vert", "shaders/lights.frag");
        planeShader.load("shaders/my.vert", "shaders/shadowPlane.frag");
        skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");
        needsReload = false;
    }
}

void ofAppPlane::updateCameraRotation(float dx, float dy)
{
    using namespace glm;

    cameraPitch -= dy;
    // -89 to +89 degrees, converted to radians
    cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));

    cameraHead -= dx;
}

//--------------------------------------------------------------
void ofAppPlane::update()
{
    if (needsReload)
    {
        reloadShaders();
    }

    float dt{ static_cast<float>(ofGetLastFrameTime()) };

    // Update position using velocity and dt.
    using namespace glm;
    camera.position += mat3(rotate(cameraHead, vec3(0, 1, 0)))
        * velocity * dt;
    camera.rotation = rotate(cameraHead, vec3(0, 1, 0)) *
        rotate(cameraPitch, vec3(1, 0, 0));

    // Update positions of moving meshes
    time += dt;
    shieldPosition = vec3(sin(time), 1, 0);
    staffPosition = vec3(2 * sin(time), 0, 1);
    cylinderPosition = vec3(-3, 1, -sin(time));
}


void ofAppPlane::drawMesh(const CameraMatrices& camMatrices,
    const SpotLight& spotLight,
    const DirectionalLight& dirLight,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
{
    // Drawing a mesh that is taking in multiple light sources (in this case, spot and directional lights)
    using namespace glm;

    // assumes shader is already active
    shader.setUniform3f("spotLightPos", spotLight.position);
    shader.setUniform3f("spotLightConeDir", spotLight.direction);
    shader.setUniform1f("spotLightCutoff", spotLight.cutoff);
    shader.setUniform3f("spotLightColor", spotLight.color * spotLight.intensity);

    shader.setUniform3f("dirLightDir", normalize(dirLight.direction));
    shader.setUniform3f("dirLightColor", dirLight.color * dirLight.intensity);

    shader.setUniform3f("cameraPosition", camMatrices.getCamera().position);

    shader.setUniform3f("ambientColor", ambientLight);
    shader.setUniformMatrix4f("mvp",
        camMatrices.getProj() * camMatrices.getView() * modelMatrix);
    shader.setUniformMatrix4f("model", modelMatrix);
    shader.setUniformMatrix3f("normalMatrix",
        inverse(transpose(mat3(modelMatrix))));
    mesh.draw();
}

void ofAppPlane::drawCube(const CameraMatrices& camMatrices)
{
    // Drawing skymap cube
    using namespace glm;

    skyboxShader.begin();
    glDepthFunc(GL_LEQUAL); // pass depth test at far clipping plane
    skyboxShader.setUniformMatrix4f("mvp",
        camMatrices.getProj() * mat4(mat3(camMatrices.getView())));
    skyboxShader.setUniformTexture("cubemap", cubemap.getTexture(), 0);
    cubeMesh.draw();
    skyboxShader.end();
    glDepthFunc(GL_LESS);
}

void ofAppPlane::drawScene(CameraMatrices& camMatrices, int reflection)
{
    // Drawing the scene relative to a specific camera
    using namespace glm;

    // Define the directional light
    DirectionalLight dirLight{ };
    dirLight.direction = normalize(vec3(1, 1, 1));
    dirLight.color = vec3(0.1, 0.9, 0.1);  // white light
    dirLight.intensity = 1;
    dirLightIntensity = dirLight.intensity;

    // Define the spot light
    SpotLight spotLight{ };
    spotLight.position = vec3(0, 2, 2);
    spotLight.direction = vec3(0, -1, -1);
    spotLight.cutoff = cos(radians(30.0f /* degrees */));
    spotLight.color = vec3(0.9, 0.9, 0.9);  // blue light
    spotLight.intensity = 5;
    spotLightIntensity = spotLight.intensity;
    spotLightPos = spotLight.position;
    spotLightDir = spotLight.direction;

    lightShader.begin();
    // Set up the textures in advance
    lightShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    lightShader.setUniformTexture("normalTex", shieldNormal, 1);
    lightShader.setUniform1f("usedForTerrain", 0);

    // Drawing only meshes that should currently be displayed
    lightShader.setUniform1f("hasTexture", 1);
    if (isShieldDrawn)
        drawMesh(camMatrices, spotLight, dirLight, vec3(0.10),
            lightShader, shieldMesh,
            translate(vec3(shieldPosition)));
    lightShader.setUniform1f("hasTexture", 0);
    if (isStaffDrawn)
        drawMesh(camMatrices, spotLight, dirLight, vec3(0.10),
            lightShader, staffMesh,
            translate(vec3(staffPosition)));
    if (isConeDrawn)
        drawMesh(camMatrices, spotLight, dirLight, vec3(0.10),
            lightShader, coneMesh,
            translate(vec3(conePosition)));
    if (isCylinderDrawn)
        drawMesh(camMatrices, spotLight, dirLight, vec3(0.10),
            lightShader, cylinderMesh,
            translate(vec3(cylinderPosition)));
    if (isTorusDrawn)
        drawMesh(camMatrices, spotLight, dirLight, vec3(0.10),
            lightShader, torusMesh,
            translate(vec3(torusPosition)));

    lightShader.end();


    drawCube(camMatrices);
}

//--------------------------------------------------------------
void ofAppPlane::draw()
{
    // Main draw function
    using namespace glm;

    // Setting values for aspect and near and far planes
    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect = width / height;
    float nearPlane = 1.0f;
    float farPlane = 20.0f;

    // Setting up both FBO cameras
    mat4 dirShadowView = mat4(mat3(lookAt(vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 1, 0))));
    mat4 dirShadowProj = ortho(-4.0f, 4.0f, -4.0f, 4.0f, -100.0f, 100.0f);

    mat4 spotShadowView = (lookAt(spotLightPos, spotLightPos + spotLightDir, vec3(0, 1, 0)));
    mat4 spotShadowProj = perspective((radians(90.0f)), aspect, nearPlane, farPlane);

    CameraMatrices dirFboCamMatrices{ dirShadowView, dirShadowProj };
    CameraMatrices spotFboCamMatrices{ spotShadowView, spotShadowProj };

    // Setting up the main camera
    CameraMatrices camMatrices{ camera, aspect, 0.01f, 40.0f };

    // Debugging by setting controlled camera to either FBO camera
    //camMatrices = dirFboCamMatrices;
    //camMatrices = spotFboCamMatrices;


    // offscreen rendering pass
    fbo.begin();
    ofClear(0, 0, 0, 0);
    drawScene(dirFboCamMatrices, 1);
    fbo.end();

    fbo2.begin();
    ofClear(0, 0, 0, 0);
    drawScene(spotFboCamMatrices, 1);
    fbo2.end();

    drawScene(camMatrices, 0);

    // Using plane shader to cast shadows onto the plane
    planeShader.begin();
    planeShader.setUniform3f("meshColor", vec3(1));
    planeShader.setUniform1f("usedForTerrain", 0);

    planeShader.setUniform3f("ambientColor", vec3(0.25));

    planeShader.setUniform3f("spotLightPos", spotLightPos);
    planeShader.setUniform3f("spotLightConeDir", spotLightDir);
    planeShader.setUniform1f("spotLightCutoff", cos(radians(10.0f)));
    planeShader.setUniform3f("spotLightColor", vec3(0.9, 0.9, 0.9) * spotLightIntensity);

    planeShader.setUniform3f("dirLightDir", normalize(vec3(1, 1, 1)));
    planeShader.setUniform3f("dirLightColor", vec3(0.1, 0.9, 0.1) * dirLightIntensity);

    planeShader.setUniform3f("cameraPos", camMatrices.getCamera().position);
    planeShader.setUniformMatrix4f("mvp", camMatrices.getProj() * camMatrices.getView());
    planeShader.setUniformMatrix4f("model", mat4());
    planeShader.setUniformMatrix3f("normalMatrix", mat3());

    planeShader.setUniformMatrix4f("shadowMatrix",
        dirFboCamMatrices.getProj() * dirFboCamMatrices.getView());

    planeShader.setUniformMatrix4f("shadowMatrix2",
        spotFboCamMatrices.getProj() * spotFboCamMatrices.getView());

    planeShader.setUniformTexture("fboTexture", fbo.getDepthTexture(), 0);
    planeShader.setUniformTexture("fboTexture2", fbo2.getDepthTexture(), 1);
    planeMesh.draw();
    planeShader.end();

    // Draw the frame time last so it doesn't get drawn over
    drawFrameTime();
}

//--------------------------------------------------------------
void ofAppPlane::keyPressed(int key)
{
    using namespace glm;

    // Forward and backward
    if (key == OF_KEY_UP || key == 'w')
    {
        velocity.z = -1;
    }
    else if (key == OF_KEY_DOWN || key == 's')
    {
        velocity.z = 1;
    }

    // Left and right
    else if (key == OF_KEY_LEFT || key == 'a')
    {
        velocity.x = -1;
    }
    else if (key == OF_KEY_RIGHT || key == 'd')
    {
        velocity.x = 1;
    }

    // Up and down
    else if (key == 'r')
    {
        velocity.y = 1;
    }
    else if (key == 'f')
    {
        velocity.y = -1;
    }

    // Setting visibility of meshes with 1-5
    if (key == '1')
    {
        isShieldDrawn = !isShieldDrawn;
    }
    if (key == '2')
    {
        isStaffDrawn = !isStaffDrawn;
    }
    if (key == '3')
    {
        isConeDrawn = !isConeDrawn;
    }
    if (key == '4')
    {
        isCylinderDrawn = !isCylinderDrawn;
    }
    if (key == '5')
    {
        isTorusDrawn = !isTorusDrawn;
    }

    // Allowing or disallowing mouse movement
    if (key == 'e')
    {
        allowMouseMovement = !allowMouseMovement;
    }
}

//--------------------------------------------------------------
void ofAppPlane::keyReleased(int key)
{
    if (key == '`')
    {
        // Reload shaders
        needsReload = true;
    }

    // Reseting velocity when keys are released
    else if (key == OF_KEY_LEFT || key == 'a' || key == OF_KEY_RIGHT || key == 'd')
    {
        velocity.x = 0;
    }
    else if (key == OF_KEY_UP || key == 'w' || key == OF_KEY_DOWN || key == 's')
    {
        velocity.z = 0;
    }
    else if (key == 'r' || key == 'f')
    {
        velocity.y = 0;
    }
}

//--------------------------------------------------------------
void ofAppPlane::mouseMoved(int x, int y)
{
    // update camera rotation only when allowMouseMovement is true
    if (allowMouseMovement)
        if (prevX != 0 && prevY != 0)
        {
            // Previous mouse position has been initialized.
            // Calculate dx and dy
            int dx = x - prevX;
            int dy = y - prevY;

            updateCameraRotation(mouseSensitivity * dx, mouseSensitivity * dy);
        }

    prevX = x;
    prevY = y;
}

//--------------------------------------------------------------
void ofAppPlane::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofAppPlane::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofAppPlane::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofAppPlane::mouseEntered(int x, int y)
{

}

//--------------------------------------------------------------
void ofAppPlane::mouseExited(int x, int y)
{

}

//--------------------------------------------------------------
void ofAppPlane::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofAppPlane::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofAppPlane::dragEvent(ofDragInfo dragInfo)
{

}

// Function to draw the frame time
void ofAppPlane::drawFrameTime()
{
    glDisable(GL_CULL_FACE);
    std::string frameTimeStr{
        to_string(ofGetLastFrameTime() * 1000.0)
            + "ms\n" + to_string(ofGetFrameRate()) + "fps"
    };
    ofDrawBitmapStringHighlight(frameTimeStr, glm::vec2(50, 50));
    //glEnable(GL_CULL_FACE);
}
