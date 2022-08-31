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

	ui.setName("ofApp");
	ui.setup();

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

	ui.Begin();
	{
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (ui.BeginWindow("ofApp"))
		{
			ui.Add(player.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);

			ui.EndWindow();
		}
	}
	ui.End();
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
