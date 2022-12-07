#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "ofxCubemap.h"
#include "CameraMatrices.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	// Shader for a directional light
	ofShader directionalLightShader;

	// Shader for a directional light
	ofShader pointLightShader;

	// Shader for a spot light
	ofShader spotLightShader;

	// Mesh of a shield
	ofMesh shieldMesh;

	ofMesh swordMesh;
	glm::vec3 swordPosition{ 0, 1, 3 };

	ofMesh staffMesh;
	glm::vec3 staffPosition{ 3, 0, 0 };

	ofMesh cupMesh;
	glm::vec3 cupPosition{ 0, 1, 0 };

	// diffuse (color) texture
	ofImage shieldDiffuse;

	// normal map
	ofImage shieldNormal;

	// specular texture
	ofImage shieldSpecular;

	ofImage swordDiffuse;

	ofImage swordNormal;

	// Mesh for the skybox
	ofMesh cubeMesh;

	// Cubemap texture
	ofxCubemap cubemap;

	// Shader for rendering the skybox with a cubemap
	ofShader skyboxShader;

	void drawCube(const CameraMatrices& camMatrices);

	// Do shaders need to be reloaded?
	bool needsReload { true };

	// Load the shaders for this app
	void reloadShaders();

	// Keep track of previous mouse position
	int prevX { 0 }, prevY { 0 };

	// Allows us to configure how much moving the mouse affects the rotation
	float mouseSensitivity { 0.02f };

	// Called to update rotation of the camera from mouse movement
	void updateCameraRotation(float dx, float dy);

	// Euler transformation
	float cameraHead { 0 };
	float cameraPitch { 0 };

	// Camera position
	Camera camera { glm::vec3(0, 1, 2) };

	// Camera velocity
	glm::vec3 velocity {};

	// Animate shield position
	float time { 0 };
	glm::vec3 shieldPosition {0, 1, 0};

	// Plane shader
	ofShader planeShader {};

	// Plane mesh
	ofMesh planeMesh {};

	// off-screen framebuffer object
	ofFbo fbo {};

	// Draws the whole scene using the currently active framebuffer
	void drawScene(CameraMatrices& camMatrices, int reflection);


};
