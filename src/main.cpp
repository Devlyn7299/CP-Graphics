#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
<<<<<<< Updated upstream
    ofGLWindowSettings glSettings;
    glSettings.setSize(1024, 768);
    glSettings.windowMode = OF_WINDOW;
    glSettings.setGLVersion(4, 1);
    ofCreateWindow(glSettings);

    printf("%s\n", glGetString(GL_VERSION));
    ofRunApp(new ofApp());
=======
	ofGLWindowSettings glSettings {};
    glSettings.setSize(1024, 768);
    glSettings.windowMode = OF_WINDOW;
    glSettings.setGLVersion(4, 1); // Use OpenGL 4.1
    auto window { ofCreateWindow(glSettings) };

    // Uncomment the following line to put the game in full screen mode.
    // window->setFullscreen(true);

	// This kicks off the running of the app
	ofRunApp(new ofApp());
>>>>>>> Stashed changes
}
