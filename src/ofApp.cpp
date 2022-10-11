#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofDisableArbTex(); 

	reloadShaders();

	ofEnableDepthTest();

	// Loading in oject1 and object2 and getting them set up
	object1.load("NewTest.ply");
	object2.load("Staff.ply");
	object1.flatNormals();
	object2.flatNormals();

	for (size_t i{ 0 }; i < object1.getNumNormals(); i++)
	{
		object1.setNormal(i, -object1.getNormal(i));
	}	
	for (size_t i{ 0 }; i < object2.getNumNormals(); i++)
	{
		object2.setNormal(i, -object2.getNormal(i));
	}

	objectVbo1.setMesh(object1, GL_STATIC_DRAW);
	objectVbo2.setMesh(object2, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::update() {
	// Reload if needed
	if (needsReload)
	{
		reloadShaders();
	}

	float dt{ static_cast<float>(ofGetLastFrameTime()) };

	// Setting velocity world space and adding it to the camera position every update
	glm::vec3 velocityWorldSpace{ glm::mat3((rotate(-cam.cameraHead, glm::vec3(0, 1, 0)) * rotate(-cam.cameraPitch, glm::vec3(1, 0, 0)))) * cam.velocity };
	cam.pos += velocityWorldSpace * dt;
}

//--------------------------------------------------------------
void ofApp::draw() {
	using namespace glm;

	float width{ static_cast<float>(ofGetViewportWidth()) };
	float height{ static_cast<float>(ofGetViewportHeight()) };
	float aspect = width / height;

	shader.begin();
	// Stress testing with this for loop - drawing i copies of each mesh
	// Currently set up so each mesh will be 5 units away from the last
	for (int i = 0; i < 1000; i++)
	{
		// Setting up MVP
		mat4 model{ translate(vec3(-1, 0, -5*i)) * rotate(radians(0.0f), vec3(1, 1, 1)) * scale(vec3(0.5, 0.5, 0.5)) };
		mat4 view{ (rotate(cam.cameraPitch, vec3(1, 0, 0)) * rotate(cam.cameraHead, vec3(0, 1, 0))) * translate(-cam.pos) };
		mat4 proj{ perspective(radians(90.0f), aspect, 0.1f, 10.0f) };

		mat4 mvp{ proj * view * model };

		// Setting shader uniforms for MVP and ModelView
		shader.setUniformMatrix4f("mvp", mvp);
		shader.setUniformMatrix4f("modelView", view * model);

		// Drawing object1
		objectVbo1.drawElements(GL_TRIANGLES, objectVbo1.getNumIndices());

		// Giving Model a new value for object2
		model = { translate(vec3(1, 0, -5*i)) * rotate(radians(0.0f), vec3(1, 1, 10)) * scale(vec3(1, 1, 1)) };
		mvp = { proj * view * model };

		// Resetting uniforms for object2
		shader.setUniformMatrix4f("mvp", mvp);
		shader.setUniformMatrix4f("modelView", view * model);

		// Drawing object2
		objectVbo2.drawElements(GL_TRIANGLES, objectVbo2.getNumIndices());
	}
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '`') {
		needsReload = true;
	}

	// Directional keys
	if (key == 'w')
	{
		cam.velocity.z = -1;
	}
	else if (key == 's')
	{
		cam.velocity.z = 1;
	}
	else if (key == 'a')
	{
		cam.velocity.x = -1;
	}
	else if (key == 'd')
	{
		cam.velocity.x = 1;
	}

	// Added R and F to go up and down, relative to where the camera is pointing
	else if (key == 'f')
	{
		cam.velocity.y = -1;
	}
	else if (key == 'r')
	{
		cam.velocity.y = 1;
	}

	// Allowing the user to disable mouseMovement with E
	if (key == 'e')
	{
		allowMouseMovement = !allowMouseMovement;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'w' || key == 's')
	{
		cam.velocity.z = 0;
	}
	else if (key == 'a' || key == 'd')
	{
		cam.velocity.x = 0;
	}
	else if (key == 'f' || key == 'r')
	{
		cam.velocity.y = 0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
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
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::reloadShaders()
{
	if (needsReload) {
		shader.load("shader.vert", "shader.frag");
		needsReload = false;
	}
}

void ofApp::updateCameraRotation(float dx, float dy)
{
	using namespace glm;
	cam.cameraHead += dx;
	cam.cameraPitch += dy;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
