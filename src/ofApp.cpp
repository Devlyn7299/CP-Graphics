#include "ofMain.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();

	// Setting up texture coordinates on the screen
	mesh.addVertex(glm::vec3(-1, -1, 0));
	mesh.addVertex(glm::vec3(-1, 1, 0));
	mesh.addVertex(glm::vec3(1, 1, 0));
	mesh.addVertex(glm::vec3(1, -1, 0));

	mesh.addTexCoord(glm::vec2(0, 0));
	mesh.addTexCoord(glm::vec2(0, 1));
	mesh.addTexCoord(glm::vec2(1, 1));
	mesh.addTexCoord(glm::vec2(1, 0));

	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	mesh.addIndices(indices, 6);

	// Loading shader files
	shader.load("passthrough.vert", "texture.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
	// Reloading shaders (if needed) and updating both particle systems every update
	reloadShaders();
	lightningParticleSystem.update(ofGetLastFrameTime());
	cloudParticleSystem.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
	shader.begin();

	// Using Alpha blending for clouds
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	// Looping through cloud particle system
	for (BasicParticle& p : cloudParticleSystem)
	{
		// Choosing a texture
		smokeImg.load("smoke_0" + to_string(p.getTextureNum()) + ".png");

		// Setting texture type
		p.setType("cloud");

		// Setting uniform variables for the shader
		shader.setUniformTexture("tex", smokeImg, 0);
		shader.setUniform3f("translation", p.getPosition()); 
		shader.setUniform4f("addedColor", p.getColor());
		shader.setUniform1f("brightness", p.getBrightness());
		shader.setUniform3f("scale", p.getScale());

		// Drawing to the mesh
		mesh.draw();

	}
	ofDisableBlendMode();

	// Using Additive blending for lighting
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// Looping through the lightning particle system
	for (BasicParticle& p : lightningParticleSystem)
	{
		// Choosing a texture
		lightningImg.load("spark_0" + to_string(p.getTextureNum()) + ".png");

		// Setting texture type
		p.setType("lightning");

		// Setting uniform variables for the shader
		shader.setUniformTexture("tex", lightningImg, 0);
		shader.setUniform3f("translation", p.getPosition()); 
		shader.setUniform4f("addedColor", p.getColor());
		shader.setUniform1f("brightness", p.getBrightness());
		shader.setUniform3f("scale", p.getScale());

		// Drawing to the mesh
		mesh.draw();
	}
	ofDisableBlendMode();

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
