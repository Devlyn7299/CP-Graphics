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
}

//--------------------------------------------------------------
void ofApp::update() {
	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::draw() {
	using namespace glm;

	float width{ static_cast<float>(ofGetViewportWidth()) };
	float height{ static_cast<float>(ofGetViewportHeight()) };
	float aspect = width / height;

	mat4 model{ translate(vec3(0, 0, -7)) * rotate(radians(80.0f), vec3(1,1,1)) };
	mat4 view{};
	mat4 proj{ perspective(radians(100.0f), aspect, 0.01f, 10.0f) };
	shader.begin();
	shader.setUniformMatrix4f("mvp", proj * view * model);
	object.draw();
	model = { translate(vec3(4, 0, -7)) * rotate(radians(80.0f), vec3(1,1,1)) };

	shader.setUniformMatrix4f("mvp", proj * view * model);
	object2.draw();
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '`') {
		needsReload = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
