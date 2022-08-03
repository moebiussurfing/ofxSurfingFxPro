#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() 
{
	w.setFrameRate(60);
	w.setVerticalSync(false);

	ofSetBackgroundColor(20);

	setupPlayer();

	//--

	// App Session Settings

	params_ofApp.add(bGui);
	params_ofApp.add(player.bGui);

	//--

	// Gui

	guiManager.setName("ofApp");
	guiManager.setup();

	//--

	// Startup

	ofxSurfingHelpers::load(params_ofApp);
}

//--------------------------------------------------------------
void ofApp::setupPlayer() 
{
	player.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw() 
{
	if (!bGui) return;

	ofDisableDepthTest();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	// ofApp ImGui Window

	guiManager.begin();
	{
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (guiManager.beginWindow("ofApp"))
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
	if (key == 'G') bGui = !bGui; // Toggle all GUI
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	player.dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::exit() 
{
	ofxSurfingHelpers::save(params_ofApp);
}
