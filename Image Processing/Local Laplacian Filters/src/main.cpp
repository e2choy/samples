#include "app.h"
#include "ofAppGlutWindow.h"

int main()
{
	ofAppGlutWindow window; 
	//ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofSetupOpenGL(&window, 512, 512, OF_WINDOW);
	ofRunApp(new Comp5900::App()); 
	return EXIT_SUCCESS;
}
