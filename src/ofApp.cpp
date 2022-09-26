#include "ofMain.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();

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

	shader.load("passthrough.vert", "texture.frag");
	
	lightningImg.load("textures/spark_04.png");
	smokeImg.load("textures/smoke_02.png");

}

//--------------------------------------------------------------
void ofApp::update(){
	reloadShaders();
	lightningParticleSystem.update(ofGetLastFrameTime());
	cloudParticleSystem.update(ofGetLastFrameTime());
	//dt += ofGetLastFrameTime();


}

//--------------------------------------------------------------
void ofApp::draw(){

	shader.begin();
	int count = 0;

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	for (BasicParticle& p : cloudParticleSystem)
	{
		smokeImg.load("textures/smoke_0" + to_string(p.getTextureNum()) + ".png");

		p.setType("cloud");
		//shader.setUniform("particlePosition", p.getPosition());
		//shader.setUniform("particleSize", /*???*/);
		//shader.setUniformTexture("particlePosition", img, 1);

		if (p.getLife() <= 0)
		{
			//cout << "no" << endl;
			//shader.setUniform1f("brightness", -100.0f);
			//particleGenerator.respawn(p);
			//brightness -= 0.05f;
		}

		//if (p.getLife() > 0.75f)
		//{
		//	//cout << "yeah" << endl;
		//	p.setBrightness(p.getBrightness() - 50.0f);
		//	if (p.getBrightness() < 1.0f) {
		//		p.setBrightness(1.0f);
		//	}
		//}

		shader.setUniformTexture("spark", smokeImg, 0);
		shader.setUniform3f("translation", p.getPosition()); // glm::vec3(0.0, 0.0, 0.0));
		shader.setUniform4f("addedColor", p.getColor());
		shader.setUniform1f("brightness", p.getBrightness());
		shader.setUniform3f("scale", p.getScale());



		mesh.draw();

		//if (count % 2 == 0) {
		//	lightningImg.load("textures/spark_01.png");
		//}
		//else
		//	lightningImg.load("textures/spark_02.png");

		count++;
		//p.setLife(p.getLife() - dt);
		//cout << p.getLife() << endl;

		//cout << count;
	}
	ofDisableBlendMode();

	ofEnableBlendMode(OF_BLENDMODE_ADD);

	for (BasicParticle& p : lightningParticleSystem)
	{
		lightningImg.load("textures/spark_0" + to_string(p.getTextureNum()) + ".png");

		p.setType("lightning");
		//shader.setUniform("particlePosition", p.getPosition());
		//shader.setUniform("particleSize", /*???*/);
		//shader.setUniformTexture("particlePosition", img, 1);

		if (p.getLife() <= 0)
		{
			//cout << "no" << endl;
			//shader.setUniform1f("brightness", -100.0f);
			//particleGenerator.respawn(p);
			//brightness -= 0.05f;
		}

		//if (p.getLife() > 0.75f)
		//{
		//	//cout << "yeah" << endl;
		//	p.setBrightness(p.getBrightness() - 50.0f);
		//	if (p.getBrightness() < 1.0f) {
		//		p.setBrightness(1.0f);
		//	}
		//}

		shader.setUniformTexture("spark", lightningImg, 0);
		shader.setUniform3f("translation", p.getPosition()); // glm::vec3(0.0, 0.0, 0.0));
		shader.setUniform4f("addedColor", p.getColor());
		shader.setUniform1f("brightness", p.getBrightness());
		shader.setUniform3f("scale", p.getScale());



		mesh.draw();

		//if (count % 2 == 0) {
		//	lightningImg.load("textures/spark_01.png");
		//}
		//else
		//	lightningImg.load("textures/spark_02.png");

		count++;
		//p.setLife(p.getLife() - dt);
		//cout << p.getLife() << endl;

		//cout << count;
	}
	ofDisableBlendMode();



	//shader.setUniformTexture("spark", lightningImg, 0);
	//lightning.draw();
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

//void ofApp::updateBrightness( float dt, float life )
//{
//	if (life > 0.75f)
//	{
//		brightness -= 1.0f * dt;
//	}
//}

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
