#pragma once

#include "ofMain.h"
#include "BasicParticleGenerator.h"
#include "ParticleSystem.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		BasicParticleGenerator particleGenerator{/* any parameters go here */};
		ParticleSystem<BasicParticle> lightningParticleSystem{ particleGenerator, 5, 0.4f };
		ParticleSystem<BasicParticle> cloudParticleSystem{ particleGenerator, 10, 0.5f };
		ofMesh mesh;
		ofShader shader;
		ofImage lightningImg;
		ofImage smokeImg;


		//float brightness{ 10.0f };
		//void updateBrightness( float dt, float life );
		bool needsReload{ true };
		void reloadShaders();
		
};
