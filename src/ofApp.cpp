#include "ofMain.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	quad.addVertex(glm::vec3(-1, -1, 0));
	quad.addVertex(glm::vec3(-1, 1, 0));
	quad.addVertex(glm::vec3(1, 1, 0));
	quad.addVertex(glm::vec3(1, -1, 0));

	quad.addTexCoord(glm::vec2(0, 0));
	quad.addTexCoord(glm::vec2(0, 1));
	quad.addTexCoord(glm::vec2(1, 1));
	quad.addTexCoord(glm::vec2(1, 0));

	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	quad.addIndices(indices, 6);

	shader.load("passthrough.vert", "texture.frag");
	
	ofDisableArbTex();
	img.load("spark_01.png");
}

//--------------------------------------------------------------
void ofApp::update(){
	reloadShaders();
	particleSystem.update(ofGetLastFrameTime());

}

//--------------------------------------------------------------
void ofApp::draw(){
	shader.begin();
	for (const BasicParticle& p : particleSystem)
	{
		//shader.setUniform("particlePosition", p.getPosition());
		//shader.setUniform("particleSize", /*???*/);
	}

	shader.setUniformTexture("spark", img, 0);
	quad.draw();
	shader.end();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '`') {
		needsReload = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

void ofApp::reloadShaders()
{
	if (needsReload) {
		shader.load("passthrough.vert", "texture.frag");
		needsReload = false;
	}
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
