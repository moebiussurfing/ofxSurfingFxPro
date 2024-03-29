#pragma once

//----

#include "ofMain.h"

#include "ofxSurfingFxPro.h"
#include "ofxSurfingImGui.h"
#include "ofxWindowApp.h"
#include "SurfingNDIManager.h"

//--

class ofApp : public ofBaseApp
{
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);
	void exit();

	ofxSurfingFxPro fxPro;
	ofxSurfingGui ui;
	SurfingNDIManager ndi;
	ofxWindowApp w;

	ofParameterGroup params_ofApp{ "ofApp" };
	ofParameter<bool> bGui{ "ofApp", true };

	void drawGui();
	void drawScene();

	ofEventListener listener_bGui1;
	ofEventListener listener_bGui2;
	bool bExclusive = false;
};
