#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "ofxCubemap.h"
#include "CameraMatrices.h"

struct CameraData
{
	glm::vec3 pos{ 0, 0, 1 };
	glm::vec3 velocity{};
	float cameraHead{ 0 };
	float cameraPitch{ 0 };
};

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
		CameraData cam;
		bool allowMouseMovement{ true };

		// Shader for multiple light sources
		ofShader allLightShader;

		// Mesh of a sword
		ofMesh swordMesh;

		// Diffuse (color) texture for sword
		ofImage swordDiffuse;

		// Normal map for sword
		ofImage swordNormal;

		// Metal texture for sword
		ofImage swordMetal;

		// Mesh for the skybox
		ofMesh cubeMesh;

		// Cubemap texture
		ofxCubemap cubemap;

		// Shader for rendering the skybox with a cubemap
		ofShader skyboxShader;

		void drawCube(const CameraMatrices& camMatrices);

		// Do shaders need to be reloaded
		bool needsReload{ true };

		// Load the shaders for this app
		void reloadShaders();

		// Keep track of previous mouse position
		int prevX{ 0 }, prevY{ 0 };

		// Allows us to configure how much moving the mouse affects the rotaiton
		float mouseSensitivity{ 0.01f };

		// Called to update rotation of the camera from mouse movement
		void updateCameraRotation(float dx, float dy);

		// Euler transformation
		float cameraHead { 0 };
		float cameraPitch { 0 };

		// Camera position
		Camera camera{glm::vec3(0,0,2)};

		// Camera velocity
		glm::vec3 velocity{};

		// Animate sword position
		float time{ 0 };
		glm::vec3 swordPosition{};


};
