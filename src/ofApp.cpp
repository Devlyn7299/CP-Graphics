#include "ofApp.h"
#include "CameraMatrices.h"
#include "calcTangents.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

//--------------------------------------------------------------
void ofApp::setup(){
	using namespace glm;

	ofDisableArbTex(); // IMPORTANT!

	// Enable interpolation across sides of the cubemap
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Disable Windows cursor
	auto window{ ofGetCurrentWindow() };

	ofEnableDepthTest();

	// Load shaders for the first time
	reloadShaders();

	// Load sword mesh
	swordMesh.load("models/sword.ply");
	calcTangents(swordMesh);

	// Load sword textures
	swordDiffuse.load("textures/sword_color.png");
	swordNormal.load("textures/sword_normal.png");
	swordMetal.load("textures/sword_metallic.png");

	// Load skybox mesh
	cubeMesh.load("models/cube.ply");

	// Load cubemap images
	cubemap.load("textures/skybox_front.png", "textures/skybox_back.png",
		"textures/skybox_right.png", "textures/skybox_left.png",
		"textures/skybox_top.png", "textures/skybox_bottom.png");

	//swordDiffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	//swordDiffuse.getTexture().generateMipmap();
	//swordDiffuse.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	//swordNormal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	//swordNormal.getTexture().generateMipmap();
	//swordNormal.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	cubemap.getTexture().generateMipmap();
	swordDiffuse.getTexture().generateMipmap();
	swordNormal.getTexture().generateMipmap();
}


void ofApp::reloadShaders()
{
	if (needsReload) {
		directionalLightShader.load("shaders/my.vert", "shaders/directionalLight.frag");
		pointLightShader.load("shaders/my.vert", "shaders/pointLight.frag");
		spotLightShader.load("shaders/my.vert", "shaders/spotLight.frag");
		skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");
		//shader.load("shaders/my.vert", "shaders/my.frag");
		allLightShader.load("shaders/my.vert", "shaders/allLights.frag");
		needsReload = false;
	}
}

void ofApp::updateCameraRotation(float dx, float dy)
{
	using namespace glm;
	cameraHead -= dx;
	cameraPitch -= dy;
	cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));

}


//--------------------------------------------------------------
void ofApp::update(){
	// Reload if needed
	if (needsReload)
	{
		reloadShaders();
	}

	float dt{ static_cast<float>(ofGetLastFrameTime()) };

	// Update position using velocity and dt
	using namespace glm;
	camera.position += mat3(rotate(cameraHead, vec3(0, 1, 0))) * velocity * dt;
	camera.rotation = rotate(cameraHead, vec3(0, 1, 0)) * rotate(cameraPitch, vec3(1, 0, 0));
	// Setting velocity world space and adding it to the camera position every update
	//glm::vec3 velocityWorldSpace{ glm::mat3((rotate(-cameraHead, glm::vec3(0, 1, 0)) * rotate(-cameraPitch, glm::vec3(1, 0, 0)))) * velocity };
	//camera.position += velocityWorldSpace * dt;
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

void drawMesh(const CameraMatrices& camMatrices,
	const PointLight& light,
	const glm::vec3 ambientLight,
	ofShader& shader, ofMesh& mesh,
	glm::mat4 modelMatrix = glm::mat4{})
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
	glm::mat4 modelMatrix = glm::mat4{})
{
	using namespace glm;

	// Assumes shader is already active
	shader.setUniform3f("lightPosition", light.position);
	shader.setUniform3f("lightConeDirection", light.direction);
	shader.setUniform1f("lightCutoff", light.cutoff);
	shader.setUniform3f("lightColor", light.color * light.intensity);
	shader.setUniform3f("ambientColor", ambientLight);
	shader.setUniformMatrix4f("mvp",
		camMatrices.getProj() * camMatrices.getView() * modelMatrix);
	shader.setUniformMatrix4f("model", modelMatrix);
	shader.setUniformMatrix3f("normalmatrix",
		inverse(transpose(mat3(modelMatrix))));


	//shader.setUniform3f("lightDir", normalize(vec3(1, 1, 1)));
	//shader.setUniform3f("lightColor", vec3(1, 1, 1)); // white light
	//shader.setUniform3f("meshColor", vec3(1, 0, 0)); // red material
	//shader.setUniform3f("ambientColor", vec3(0.1, 0.1, 0.1));
	//shader.setUniformMatrix4f("mvp", camMatrices.getProj() * camMatrices.getView());
	//shader.setUniformMatrix3f("normalMatrix", mat3()); // identity for model / normal matrix
	//shader.setUniformTexture("diffuseTex", swordDiffuse, 0);
	//shader.setUniformTexture("normalTex", swordNormal, 1); // IMPORTANT: Use a different slot 

	mesh.draw();
}

void drawMesh(const CameraMatrices& camMatrices,
	const SpotLight& spotLight,
	const DirectionalLight& dirLight,
	const glm::vec3 ambientLight,
	ofShader& shader, ofMesh& mesh,
	glm::mat4 modelMatrix = glm::mat4{})
{
	using namespace glm;

	// Assumes shader is already active

	// Spot light
	shader.setUniform3f("spotLightPos", spotLight.position);
	shader.setUniform3f("spotLightConeDir", spotLight.direction);
	shader.setUniform1f("spotLightCutoff", spotLight.cutoff);
	shader.setUniform3f("spotLightColor", spotLight.color * spotLight.intensity);



	shader.setUniform3f("cameraPosition", camMatrices.getCamera().position);
	shader.setUniform3f("dirLightDir", dirLight.direction);
	shader.setUniform3f("dirLightColor", dirLight.color * dirLight.intensity);
	shader.setUniform3f("ambientColor", ambientLight);
	shader.setUniformMatrix4f("mvp",
		camMatrices.getProj() * camMatrices.getView() * modelMatrix);
	shader.setUniformMatrix3f("normalMatrix",
		inverse(transpose(mat3(modelMatrix))));
	shader.setUniformMatrix4f("model", modelMatrix);
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
//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;

	float width{ static_cast<float>(ofGetViewportWidth()) };
	float height{ static_cast<float>(ofGetViewportHeight()) };
	float aspect = width / height;

	// Calculate the view and projection matrices for the camera.
	CameraMatrices camMatrices{ camera, aspect, 0.01f, 10.0f, };

	// Define the directional light
	DirectionalLight dirLight{};
	dirLight.direction = normalize(vec3(1, 1, 1));
	dirLight.color = vec3(1, 1, 1); // white light
	dirLight.intensity = 3;

	// Define the point light
	PointLight pointLight{};
	pointLight.position = vec3(0, 0, 1.5);
	pointLight.color = vec3(1, 1, 1); // white light
	pointLight.intensity = 1;

	// Define the spot light
	SpotLight spotLight{};
	spotLight.position = vec3(3, 0, 0);
	spotLight.direction = vec3(-1, 0, 0);
	spotLight.cutoff = cos(radians(15.0f /* degrees */));
	spotLight.color = vec3(1, 0.1, 0.1); // red light
	spotLight.intensity = 3;

	allLightShader.begin();

	// Set up the textures in advance
	allLightShader.setUniformTexture("diffuseTex", swordDiffuse, 0);
	allLightShader.setUniformTexture("normalTex", swordNormal, 1);
	allLightShader.setUniformTexture("specularTex", swordMetal, 2);
	allLightShader.setUniformTexture("envMap", cubemap.getTexture(), 3);
	allLightShader.setUniform1f("shininess", 64.0);
	drawMesh(camMatrices, spotLight, dirLight, vec3(0.0),
		allLightShader, swordMesh, translate(vec3(swordPosition)));

	drawCube(camMatrices);
	allLightShader.end();
	


	//shader.begin(); // make shader active jut to set uniform variables
	//shader.setUniform3f("lightDir", normalize(vec3(1, 1, 1)));
	//shader.setUniform3f("lightColor", vec3(1, 1, 1)); // white light
	//shader.setUniform3f("meshColor", vec3(1, 0, 0)); // red material
	//shader.setUniform3f("ambientColor", vec3(0.1, 0.1, 0.1));
	//shader.setUniformMatrix4f("mvp", camMatrices.getProj() * camMatrices.getView());
	//shader.setUniformMatrix3f("normalMatrix", mat3()); // identity for model / normal matrix
	//shader.setUniformTexture("diffuseTex", swordDiffuse, 0);
	//shader.setUniformTexture("normalTex", swordNormal, 1); // IMPORTANT: Use a different slot 

	//swordMesh.draw();
	//drawCube(camMatrices);

	//shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '`') {
		needsReload = true;
	}

	// Directional keys
	if (key == 'w')
	{
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
	}

	// Added R and F to go up and down, relative to where the camera is pointing
	else if (key == 'f')
	{
		velocity.y = -1;
	}
	else if (key == 'r')
	{
		velocity.y = 1;
	}

	// Allowing the user to disable mouseMovement with E
	if (key == 'e')
	{
		allowMouseMovement = !allowMouseMovement;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
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
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
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
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


