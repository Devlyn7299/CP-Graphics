#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofDisableArbTex(); //IMPORTANT

	reloadShaders();

	ofEnableDepthTest();

	object.load("NewTest.ply");
	object2.load("Staff.ply");
	object.flatNormals();
	object2.flatNormals();

	for (size_t i{ 0 }; i < object.getNumNormals(); i++)
	{
		object.setNormal(i, -object.getNormal(i));
	}	for (size_t i{ 0 }; i < object2.getNumNormals(); i++)
	{
		object2.setNormal(i, -object2.getNormal(i));
	}

	objectVbo.setMesh(object, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::update() {
	reloadShaders();

	float dt{ static_cast<float>(ofGetLastFrameTime()) };


	glm::vec3 velocityWorldSpace{ glm::mat3(rotate(-cameraHead, glm::vec3(0, 1, 0))) * cam.velocity };
	//glm::vec3 velocityWorldSpace{ glm::mat3(rotate(-cameraHead, glm::vec3(0, cameraPitch, 0))) * cam.velocity };
	cam.pos += velocityWorldSpace * dt;



}

//--------------------------------------------------------------
void ofApp::draw() {
	using namespace glm;


	float width{ static_cast<float>(ofGetViewportWidth()) };
	float height{ static_cast<float>(ofGetViewportHeight()) };
	float aspect = width / height;
	mat4 model{ rotate(radians(45.0f), vec3(1, 1, 1)) * scale(vec3(0.25, 0.25, 0.25)) };

	mat4 view{ (rotate(cameraPitch, vec3(1, 0, 0)) * rotate(cameraHead, vec3(0, 1, 0))) * translate(-cam.pos) };

	mat4 proj{ perspective(radians(90.0f), aspect, 0.01f, 10.0f) };

	mat4 mvp{ proj * view * model };

	shader.begin();
	shader.setUniformMatrix4f("mvp", mvp);
	//object.draw();
	objectVbo.drawElements(GL_TRIANGLES, objectVbo.getNumIndices());
	//model = { translate(vec3(4, 0, -5)) * rotate(radians(0.0f), vec3(1,1,1)) };

	//shader.setUniformMatrix4f("mvp", proj * view * model);
	//object2.draw();
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '`') {
		needsReload = true;
	}

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
	else if (key == 'f')
	{
		cam.velocity.y = -1;
	}
	else if (key == 'r')
	{
		cam.velocity.y = 1;
	}
	

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
	cameraHead += dx;
	cameraPitch += dy;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
