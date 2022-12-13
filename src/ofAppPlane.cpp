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

    ofDisableArbTex(); // IMPORTANT!

    // Enable interpolation across sides of the cubemap
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Disable Windows cursor
    auto window{ ofGetCurrentWindow() };
    //glfwSetInputMode(dynamic_pointer_cast<ofAppGLFWWindow>(window)
    //    ->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ofEnableDepthTest();

    // Load shaders for the first time
    reloadShaders();

    // Load shield mesh
    shieldMesh.load("models/shield.ply");
    staffMesh.load("models/staff.ply");
    jarMesh.load("models/jar.ply");
    coneMesh.load("models/cone.ply");
    cube.load("models/cube.ply");
    cylinderMesh.load("models/cylinder.ply");
    sphereMesh.load("models/sphere.ply");
    torusMesh.load("models/torus.ply");
    //cupMesh.load("models/NewTest.ply");
    calcTangents(shieldMesh);
    calcTangents(staffMesh);
    //calcTangents(cupMesh);

    // Load shield textures
    shieldDiffuse.load("textures/shield_diffuse.png");
    shieldNormal.load("textures/shield_normal.png");
    shieldSpecular.load("textures/shield_spec.png");

    swordDiffuse.load("textures/sword_color.png");
    swordNormal.load("textures/sword_normal.png");

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
    if (needsReload) {
        directionalLightShader.load("shaders/my.vert", "shaders/directionalLightPlane.frag");
        pointLightShader.load("shaders/my.vert", "shaders/pointLight.frag");
        spotLightShader.load("shaders/my.vert", "shaders/spotLight.frag");
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

    time += dt;
    shieldPosition = vec3(sin(time), 1, 0);
    staffPosition = vec3(2 * sin(time), 0, 1);
    cylinderPosition = vec3(-3, 1, -sin(time));
}

void ofAppPlane::drawMeshDir(const CameraMatrices& camMatrices,
    const DirectionalLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
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

void ofAppPlane::drawMeshPoint(const CameraMatrices& camMatrices,
    const PointLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
{
    using namespace glm;

    // assumes shader is already active
    shader.setUniform3f("lightPosition", light.position);
    shader.setUniform3f("lightColor", light.color * light.intensity);
    shader.setUniform3f("ambientColor", ambientLight);
    shader.setUniformMatrix4f("mvp",
        camMatrices.getProj() * camMatrices.getView() * modelMatrix);
    shader.setUniformMatrix4f("model", modelMatrix);
    shader.setUniformMatrix3f("normalMatrix",
        inverse(transpose(mat3(modelMatrix))));
    mesh.draw();
}

void ofAppPlane::drawMeshSpot(const CameraMatrices& camMatrices,
    const SpotLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
{
    using namespace glm;

    // assumes shader is already active
    shader.setUniform3f("lightPosition", light.position);
    shader.setUniform3f("lightConeDirection", light.direction);
    shader.setUniform1f("lightCutoff", light.cutoff);
    shader.setUniform3f("lightColor", light.color * light.intensity);
    shader.setUniform3f("ambientColor", ambientLight);
    shader.setUniformMatrix4f("mvp",
        camMatrices.getProj() * camMatrices.getView() * modelMatrix);
    shader.setUniformMatrix4f("model", modelMatrix);
    shader.setUniformMatrix3f("normalMatrix",
        inverse(transpose(mat3(modelMatrix))));
    mesh.draw();
}

void ofAppPlane::drawMeshMix(const CameraMatrices& camMatrices,
    const SpotLight& spotLight,
    const DirectionalLight& dirLight,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix)
{
    using namespace glm;

    // assumes shader is already active

    shader.setUniform3f("cameraPosition", camMatrices.getCamera().position);
    shader.setUniform3f("lightDir", dirLight.direction);
    shader.setUniform3f("lightColor", dirLight.color * dirLight.intensity);



    shader.setUniform3f("lightPosition", spotLight.position);
    shader.setUniform3f("lightConeDirection", spotLight.direction);
    shader.setUniform1f("lightCutoff", spotLight.cutoff);
    shader.setUniform3f("lightColor", spotLight.color * spotLight.intensity);


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
    using namespace glm;

    // Define the directional light
    DirectionalLight dirLight{ };
    dirLight.direction = normalize(vec3(1, 1, 1));
    dirLight.color = vec3(0.9, 0.9, 0.1);  // white light
    dirLight.intensity = 1;
    dirLightIntensity = dirLight.intensity;

    // Define the point light
    PointLight pointLight{ };
    pointLight.position = vec3(0, 0, 1.5);
    pointLight.color = vec3(1, 1, 1);  // white light
    pointLight.intensity = 1;

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

    directionalLightShader.begin();
    // Set up the textures in advance
    directionalLightShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    directionalLightShader.setUniformTexture("normalTex", shieldNormal, 1); // IMPORTANT: Use a different slot
    // directional light
    //drawMesh(camMatrices, dirLight, vec3(0.10),
    //    directionalLightShader, shieldMesh,
    //    translate(vec3(shieldPosition)));
    //
    //// Spot light
    //drawMesh(camMatrices, spotLight, vec3(0.10),
    //    directionalLightShader, shieldMesh,
    //    translate(vec3(shieldPosition)));

    // Main camera
    if (isShieldDrawn)
        drawMeshMix(camMatrices, spotLight, dirLight, vec3(0.10),
            directionalLightShader, shieldMesh,
            translate(vec3(shieldPosition)));
    if (isStaffDrawn)
        drawMeshMix(camMatrices, spotLight, dirLight, vec3(0.0),
            directionalLightShader, staffMesh,
            translate(vec3(staffPosition)));
    if (isConeDrawn)
        drawMeshMix(camMatrices, spotLight, dirLight, vec3(0.0),
            directionalLightShader, coneMesh,
            translate(vec3(conePosition)));
    if (isCylinderDrawn)
        drawMeshMix(camMatrices, spotLight, dirLight, vec3(0.0),
            directionalLightShader, cylinderMesh,
            translate(vec3(cylinderPosition)));
    if (isTorusDrawn)
        drawMeshMix(camMatrices, spotLight, dirLight, vec3(0.0),
            directionalLightShader, torusMesh,
            translate(vec3(torusPosition)));

    directionalLightShader.end();


    drawCube(camMatrices);
}

//--------------------------------------------------------------
void ofAppPlane::draw()
{


    using namespace glm;

    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect = width / height;
    float nearPlane = 1.0f;
    float farPlane = 20.0f;

    mat4 spotShadowView = (lookAt(spotLightPos, spotLightPos + spotLightDir, vec3(0, 1, 0)));
    mat4 spotShadowProj = perspective((radians(90.0f)), aspect, nearPlane, farPlane);

    // in lookAt, use spot light position for eye, position + direction for center, and (0,1,0) for up
    mat4 dirShadowView = mat4(mat3(lookAt(vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 1, 0))));
    mat4 dirShadowProj = ortho(-4.0f, 4.0f, -4.0f, 4.0f, -100.0f, 100.0f);

    // FBO camera
    CameraMatrices dirFboCamMatrices{ dirShadowView, dirShadowProj };
    CameraMatrices spotFboCamMatrices{ spotShadowView, spotShadowProj };

    // Calculate the view and projection matrices for the camera.
    CameraMatrices camMatrices{ camera, aspect, 0.01f, 40.0f };
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


    planeShader.begin();


    planeShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    planeShader.setUniformTexture("normalTex", shieldNormal, 1);
    planeShader.setUniform3f("ambientColor", vec3(0.25));

    planeShader.setUniform3f("spotLightPos", spotLightPos);
    planeShader.setUniform3f("spotLightConeDir", spotLightDir);
    planeShader.setUniform1f("spotLightCutoff", cos(radians(10.0f)));
    planeShader.setUniform3f("spotLightColor", vec3(0.9, 0.9, 0.9) * spotLightIntensity);

    planeShader.setUniform3f("dirLightDir", normalize(vec3(1, 1, 1)));
    planeShader.setUniform3f("dirLightColor", vec3(0.9, 0.9, 0.1) * dirLightIntensity);

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




    drawFrameTime();
}

//--------------------------------------------------------------
void ofAppPlane::keyPressed(int key)
{
    using namespace glm;

    if (key == OF_KEY_UP || key == 'w')
    {
        // Forward motion
        // Converting from camera space velocity to world space velocity
        velocity.z = -1;
    }
    else if (key == OF_KEY_DOWN || key == 's')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.z = 1;
    }
    else if (key == OF_KEY_LEFT || key == 'a')
    {
        // Forward motion
        // Converting from camera space velocity to world space velocity
        velocity.x = -1;
    }
    else if (key == OF_KEY_RIGHT || key == 'd')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.x = 1;
    }

    else if (key == 'r')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.y = 1;
    }
    else if (key == 'f')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.y = -1;
    }
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
    else if (key == OF_KEY_LEFT || key == 'a' || key == OF_KEY_RIGHT || key == 'd')
    {
        // Reset velocity when a key is released
        velocity.x = 0;
    }
    else if (key == OF_KEY_UP || key == 'w' || key == OF_KEY_DOWN || key == 's')
    {
        // Reset velocity when a key is released
        velocity.z = 0;
    }
    else if (key == 'r' || key == 'f')
    {
        // Reset velocity when a key is released
        velocity.y = 0;
    }
}

//--------------------------------------------------------------
void ofAppPlane::mouseMoved(int x, int y)
{
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
