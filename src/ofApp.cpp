#include "ofApp.h"
#include "CameraMatrices.h"
#include "calcTangents.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

void buildPlaneMesh(float width, float depth, float height, ofMesh& planeMesh)
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
void ofApp::setup()
{
    using namespace glm;

    ofDisableArbTex(); // IMPORTANT!

    // Enable interpolation across sides of the cubemap
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Disable Windows cursor
    auto window { ofGetCurrentWindow() };
    //glfwSetInputMode(dynamic_pointer_cast<ofAppGLFWWindow>(window)
    //    ->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ofEnableDepthTest();
    ofEnableDepthTest();

    // Enable face culling to test winding order
    glEnable(GL_CULL_FACE); 

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

    calcTangents(shieldMesh);
    calcTangents(staffMesh);

    // Load shield textures
    //shieldDiffuse.load("textures/shield_diffuse.png");
    //shieldNormal.load("textures/shield_normal.png");
    //shieldSpecular.load("textures/shield_spec.png");

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
    //buildPlaneMesh(5.0, 5.0, 0.0, planeMesh);

    // Allocate a 1024x1024 RGB+alpha framebuffer
    ofFboSettings fboSettings {};
    fboSettings.width = 1024;
    fboSettings.height = 1024;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    fbo.allocate(fboSettings);
    // Setting the backround color to a sky blue color
    ofSetBackgroundColor(ofColor(135, 206, 235));   // Sky blue

    // Loading low resolution heightmap
    heightmap.setUseTexture(false);
    heightmap.load("TamrielLowRes.png");
    assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

    // Loading high resolution heightmap
    heightmapHiRes.setUseTexture(false);
    heightmapHiRes.load("TamrielHighRes.png");
    assert(heightmapHiRes.getWidth() != 0 && heightmapHiRes.getHeight() != 0);



    // Building the terrain mesh for the low resolution heightmap
    buildTerrainMesh(terrainMesh, heightmap, 0, 0,
        heightmap.getWidth() - 1, heightmap.getHeight() - 1,
        glm::vec3(32, 50*32, 32));

    // Setting the camera's position in the world
    glm::vec3 newPosition = glm::vec3(
        ((heightmap.getWidth() - 1) * 0.5f) * 32,
        30 * 32,
        ((heightmap.getHeight() - 1) * 0.5f) * 32
    );
    position = newPosition;

    // Building the terrain mesh for the high resolution heightmap (needs camera position)
    cellManager.initializeForPosition(position);

    // Creating the water mesh
    water.addVertex(glm::vec3(0, 700, 0));
    water.addVertex(glm::vec3(0, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, heightmapHiRes.getHeight() - 1));
    water.addVertex(glm::vec3(heightmapHiRes.getWidth() - 1, 700, 0));

    ofIndexType indices[6] = { 0,1,2,2,3,0 };
    water.addIndices(indices, 6);
}

void ofApp::reloadShaders()
{
    directionalLightShader.load("shaders/my.vert", "shaders/directionalLight.frag");
    pointLightShader.load("shaders/my.vert", "shaders/pointLight.frag");
    spotLightShader.load("shaders/my.vert", "shaders/spotLight.frag");
    planeShader.load("shaders/my.vert", "shaders/shadow.frag");
    skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");
    needsReload = false;
}

void ofApp::updateCameraRotation(float dx, float dy)
{
    using namespace glm;

    cameraPitch -= dy;
    // -89 to +89 degrees, converted to radians
    cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));

    cameraHead -= dx;
}

//--------------------------------------------------------------
void ofApp::update()
{
    using namespace glm;

    if (needsReload)
    {
        reloadShaders();
    }

    float dt { static_cast<float>(ofGetLastFrameTime()) };

    // Update position using velocity and dt.
    using namespace glm;
    camera.position += mat3(rotate(cameraHead, vec3(0, 1, 0)))
        * velocity * dt;
    camera.rotation = rotate(cameraHead, vec3(0, 1, 0)) *
        rotate(cameraPitch, vec3(1, 0, 0));

    time += dt;
    staffPosition = vec3(sin(time), 0, 1);
    cylinderPosition = vec3(-3, 1, sin(time));

}

void drawMesh(const CameraMatrices& camMatrices, 
    const DirectionalLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh, 
    glm::mat4 modelMatrix = glm::mat4 {})
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
    // calculate world space velocity
    vec3 velocityWorldSpace{ mat3(rotate(-cameraHead, vec3(0, 1, 0)) * rotate(-cameraPitch, vec3(1, 0, 0))) * velocity};

void drawMesh(const CameraMatrices& camMatrices,
    const PointLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix = glm::mat4 {})
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

void drawMesh(const CameraMatrices& camMatrices,
    const SpotLight& light,
    const glm::vec3 ambientLight,
    ofShader& shader, ofMesh& mesh,
    glm::mat4 modelMatrix = glm::mat4 {})
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

void ofApp::drawCube(const CameraMatrices& camMatrices)
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

void ofApp::drawScene(CameraMatrices& camMatrices, int reflection)
{
    using namespace glm;

    // Define the directional light
    DirectionalLight dirLight { };
    dirLight.direction = normalize(vec3(1, 1, 1));
    dirLight.color = vec3(1, 1, 0);  // white light
    dirLight.intensity = 1;

    // Define the point light
    PointLight pointLight { };
    pointLight.position = vec3(0, 0, 1.5);
    pointLight.color = vec3(1, 1, 1);  // white light
    pointLight.intensity = 1;

    // Define the spot light
    SpotLight spotLight { };
    spotLight.position = vec3(0, 0, 1.5);
    spotLight.direction = vec3(0, 0, -1);
    spotLight.cutoff = cos(radians(15.0f /* degrees */));
    spotLight.color = vec3(1, 1, 1);  // white light
    spotLight.intensity = 1;

    directionalLightShader.begin();

    // Set up the textures in advance
    directionalLightShader.setUniformTexture("diffuseTex", shieldDiffuse, 0);
    directionalLightShader.setUniformTexture("normalTex", shieldNormal, 1); // IMPORTANT: Use a different slot
    directionalLightShader.setUniformTexture("specularTex", shieldSpecular, 2);
    directionalLightShader.setUniformTexture("envMap", cubemap.getTexture(), 3);
    directionalLightShader.setUniform1f("shininess", 64.0);
    directionalLightShader.setUniform1i("reflection", reflection);

    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, shieldMesh, 
        translate(vec3(shieldPosition)));
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, staffMesh,
        translate(vec3(staffPosition)));
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, coneMesh,
        translate(vec3(conePosition)));
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, cylinderMesh,
        translate(vec3(cylinderPosition)));
    drawMesh(camMatrices, dirLight, vec3(0.0),
        directionalLightShader, torusMesh,
        translate(vec3(torusPosition)));

    directionalLightShader.end();

    drawCube(camMatrices);
    // Update the cellManager to account for camera position changing
    cellManager.optimizeForPosition(position);

}

//--------------------------------------------------------------
void ofApp::draw()
{
    using namespace glm;

    float width { static_cast<float>(ofGetViewportWidth()) };
    float height { static_cast<float>(ofGetViewportHeight()) };
    float aspect = width / height;

    mat4 shadowView = mat4(mat3(lookAt(vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 1, 0))));
    float shad = 6.0f;
    mat4 shadowProj = glm::ortho(-shad, shad, -shad, shad, -shad, shad);

    // FBO camera
    CameraMatrices fboCamMatrices { shadowView, shadowProj };

    // Calculate the view and projection matrices for the camera.
    //CameraMatrices camMatrices{ camera, aspect, 0.01f, 4.0f };
    CameraMatrices camMatrices { camera, aspect, 0.01f, 100.0f };

    // offscreen rendering pass
    fbo.begin();
    ofClear(0, 0, 0, 0);
    drawScene(fboCamMatrices, 1);
    fbo.end();


    drawScene(camMatrices, 0);

    planeShader.begin();
    planeShader.setUniform3f("cameraPos", camMatrices.getCamera().position);
    planeShader.setUniformMatrix4f("mvp", camMatrices.getProj() * camMatrices.getView());
    planeShader.setUniformMatrix4f("model", mat4());
    planeShader.setUniformMatrix3f("normalMatrix", mat3());
    planeShader.setUniformMatrix4f("shadowMatrix", 
        fboCamMatrices.getProj() * fboCamMatrices.getView());
    planeShader.setUniformTexture("fboTexture", fbo.getDepthTexture(), 0);
    planeMesh.draw();
    planeShader.end();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    // Setting variables for the near, mid, and far planes
    float nearPlane = 1.0f;
    float midPlane = 250.0f;
    float farPlane = 3000.0f;

    // Setting variables for the aspect ratio of the window
    float width{ static_cast<float>(ofGetViewportWidth()) };
    float height{ static_cast<float>(ofGetViewportHeight()) };
    float aspect{ width / height };

    // Setting initial model, view, and projection matrices
    mat4 model{ rotate(radians(0.0f), vec3(1, 1, 1)) * scale(vec3(1,1,1)) };
    mat4 view{ (rotate(cameraPitch, vec3(1, 0, 0)) * rotate(cameraHead, vec3(0, 1, 0))) * translate(-position) };
    // First projection matrix is set for the low resolution texture (mid plane to far plane)
    mat4 proj{ perspective(radians(90.0f), aspect, midPlane - 249.0f, farPlane) };

    // Setting initial mvp and modelView matrices
    mat4 mvp{ proj * view * model };
    mat4 modelView{ view * model };

    shader.begin(); // start using the shader

    // Setting uniform variables for the vertex shader
    shader.setUniformMatrix4f("mvp", mvp);
    shader.setUniformMatrix4f("modelView", modelView);
    shader.setUniformMatrix3f("normalMatrix", mat3(model));

    // Setting uniform variables for smoothstep use in the fragment shader
    shader.setUniform1f("nearPlane", 0);
    shader.setUniform1f("midPlane", 0);
    shader.setUniform1f("needsSmoothstep", 0);

    // Setting uniform variables for lighting use in the fragment shader
    shader.setUniform3f("meshColor", vec3(86.0f/255.0f, 125.0f/255.0f, 70.0f/255.0f));  // Grass green
    shader.setUniform3f("lightColor", vec3(0.5));
    shader.setUniform3f("lightDir", normalize(vec3(-1, 1, 1)));
    shader.setUniform3f("ambientColor", vec3(0.1, 0.2, 0.1));

    // Drawing the low resolution terrain mesh and mid-to-far plane water
    terrainMesh.draw();
    shader.setUniform3f("meshColor", vec3(11.0f / 255.0f, 199.0f / 255.0f, 250.0f / 255.0f));   // Water
    water.draw();
    
    // Clearing depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Setting new projection for near-to-mid plane
    proj = { perspective(radians(90.0f), aspect, nearPlane, midPlane + 500.0f) };
    mvp = { proj * view * model };

    shader.setUniformMatrix4f("mvp", mvp);

    // Updating uniform variables for smoothstep use in the fragment shader
    shader.setUniform1f("nearPlane", nearPlane);
    shader.setUniform1f("midPlane", midPlane + 500.0f);
    shader.setUniform1f("needsSmoothstep", 1);

    // Drawing the near-to-mid plane water and high resolution terrain mesh
    water.draw();
    shader.setUniform3f("meshColor", vec3(86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f));    // Grass green
    cellManager.drawActiveCells(position, midPlane + 250.0f);

    shader.end(); // done with the shader
    ofDisableBlendMode();

}

void ofApp::exit()
{
    cellManager.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    using namespace glm;

    if (key == OF_KEY_UP || key == 'w')
    {
        // Forward motion
        // Converting from camera space velocity to world space velocity
        velocity.z = -1;
        velocity.z = -50;
    }
    else if (key == OF_KEY_DOWN || key == 's')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.z = 1;
        velocity.z = 50;
    }
    else if (key == OF_KEY_LEFT || key == 'a')
    {
        // Forward motion
        // Converting from camera space velocity to world space velocity
        velocity.x = -1;
        velocity.x = -50;
    }
    else if (key == OF_KEY_RIGHT || key == 'd')
    {
        // Backwards motion
        // Converting from camera space velocity to world space velocity
        velocity.x = 1;
    }
    else if (key == 'r')
    {
        // Upwards motion
        // Converting from camera space velocity to world space velocity
        velocity.y = 1;
        velocity.x = 50;
    }

    // Added R and F to go up and down, relative to where the camera is pointing
    else if (key == 'f')
    {
        velocity.y = -50;
    }
    else if (key == 'r')
    {
        velocity.y = 50;
    }
    else if (key == 'f')
    {
        // Downwards motion
        // Converting from camera space velocity to world space velocity
        velocity.y = -1;
    }

    if (key == '`')
    {
        needsReload = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
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
void ofApp::mouseMoved(int x, int y)
{
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
