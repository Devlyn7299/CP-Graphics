#pragma once

#include "ofMain.h"

struct CameraData
{
	glm::vec3 pos{ 0, 0, 1 };
	float fov;
	glm::vec3 velocity{};

};

class ofApp : public ofBaseApp {

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
	ofMesh triangle{};
	ofMesh quad{};
	ofShader shader{};
	ofImage img{};
	ofMesh object;
	ofMesh object2;
	ofVbo objectVbo1;
	ofVbo objectVbo2;

	bool needsReload{ true };
	void reloadShaders();
	CameraData cam;

	int prevX{ 0 }, prevY{ 0 };

	float mouseSensitivity{ 0.01f };

	float cameraHead{ 0 };
	float cameraPitch{ 0 };

	void updateCameraRotation(float dx, float dy);

	bool allowMouseMovement{ true };

};
