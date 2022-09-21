#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("FxPro_NDI");

	w.setFrameRate(60);
	w.setVerticalSync(false);

	ofSetBackgroundColor(20);

	//--

	// Gui
	ui.setName("ofApp");
	ui.setup();

	// NDI Manager
	ndi.setup();

	params_ofApp.add(bGui);
	params_ofApp.add(ndi.bGui);
	params_ofApp.add(fxPro.bGui);

	//--

	listener_bGui1 = ndi.bGui.newListener([this](bool& b) {
		if (b && bExclusive) fxPro.bGui = false;
		});

	listener_bGui2 = fxPro.bGui.newListener([this](bool& b) {
		if (b && bExclusive) ndi.bGui = false;
		});

	//--

	// Startup
	ofxSurfingHelpers::load(params_ofApp);
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Input

	// FxPro
	// Source
	{
		fxPro.begin();
		{
			// Feed NDI In 1, NDI In 2 and/or Webcam source signals to FX processor input.
			// Which sources will be selected by the GUI

			ndi.drawSignalsFullScreen();
		}
		fxPro.end(false);
	}

	//--

	// Output
	// Processed
	{
		// Feed processed signal to NDI Out sender
		ndi.begin_NDI_OUT();
		{
			fxPro.draw();
		}
		ndi.end_NDI_OUT();
	}
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
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ImGui::Checkbox("Exclusive", &bExclusive);

			ui.AddSpacingBig();

			ui.Add(fxPro.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);

			ui.AddSpacing();

			ui.Add(ndi.bGui, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);

			ui.EndWindow();
		}
	}
	ui.End();

	// FxPro
	fxPro.drawGui();

	// NDI Manager
	ndi.drawGui();
	//ndi.draw(); // -> All the canvas layout with mini previews.
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
