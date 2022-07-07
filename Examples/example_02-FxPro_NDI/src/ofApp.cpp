#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	w.setFrameRate(60);
	w.setVerticalSync(false);

	ofSetBackgroundColor(20);

	//--

	// Gui
	guiManager.setName("ofApp");
	guiManager.setup();

	// NDI Manager
	ndi.setup();
	params_ofApp.add(ndi.bGui);
	params_ofApp.add(ndi.bGui_Controls);

	params_ofApp.add(bGui);

	//--
	
	// Startup
	ofxSurfingHelpers::load(params_ofApp);
}

//--------------------------------------------------------------
void ofApp::update()
{
	// FxPro
	{
		fxPro.begin();
		{
			// Feed NDI In 1, NDI In 2 and/or Webcam source signals to FX processor input.
			// Which sources will be selected by the GUI
			
			ndi.draw();

			//ndi.draw_NDI_IN_1();
			//ndi.draw_NDI_IN_2();
			//ndi.draw_Webcam();

			//ndi.drawSignalsFullScreen();
			//ndi.drawSignals();
		}
		fxPro.end(false);
	}

	// Feed processed signal to NDI Out sender
	ndi.begin_NDI_OUT();
	{
		fxPro.draw();
	}
	ndi.end_NDI_OUT();
}

//--------------------------------------------------------------
void ofApp::draw() 
{
	drawScene();

	if (!bGui) return;
	ofDisableDepthTest();
	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// FxPro
	// Draw processed image:
	fxPro.draw();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	// ofApp ImGui Window
	guiManager.begin();
	{
		if (guiManager.beginWindow("ofApp"))
		{
			guiManager.Add(fxPro.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);
			guiManager.AddSpacingBigSeparated();

			guiManager.Add(ndi.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);
			if(ndi.bGui) guiManager.Add(ndi.bGui_Controls, OFX_IM_TOGGLE);

			guiManager.endWindow();
		}
	}
	guiManager.end();

	// FxPro
	fxPro.drawGui();

	// NDI Manager
	//ndi.draw(); // -> canvas layout
	ndi.drawGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Hide all GUI
	if (key == 'G') bGui = !bGui;

	fxPro.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	fxPro.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	fxPro.windowResized(w, h);
	ndi.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofxSurfingHelpers::save(params_ofApp);
}
