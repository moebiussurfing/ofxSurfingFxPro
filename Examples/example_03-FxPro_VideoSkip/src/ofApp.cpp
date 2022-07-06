#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setupPlayer() 
{
	ofSetWindowTitle("videoSkip");

	//windowApp.setEnableTimerSaver(true);

	player.setup();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	player.dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::setup() {
	w.setFrameRate(50);
	w.setVerticalSync(false);

	ofSetBackgroundColor(20);

	setupPlayer();

	//--

	// App Session Settings

	params_ofApp.add(bGui);

	ofxSurfingHelpers::load(params_ofApp);

	//--

	// Gui
	guiManager.setName("videoSkip");
	guiManager.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw() {

	//----

	if (!bGui) return;

	ofDisableDepthTest();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	//--

	// ofApp ImGui Window
	guiManager.begin();
	{
		// Constraint size
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (guiManager.beginWindow("videoSkip"))
		{
			guiManager.Add(player.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);
			guiManager.endWindow();
		}
	}
	guiManager.end();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Hide all GUI
	if (key == 'G') bGui = !bGui;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}


//--------------------------------------------------------------
void ofApp::exit() {
	ofxSurfingHelpers::save(params_ofApp);
}
