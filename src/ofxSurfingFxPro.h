#pragma once

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

class ofxSurfingFxPro {

public:

	ofxSurfingFxPro();
	~ofxSurfingFxPro();

	void setup();
	void update();
	//void draw();
	void drawGui();
	void keyPressed(int key);
	void windowResized(int w, int h);
	void exit();

	void begin();
	void begin(ofCamera& cam);
	void end();

	ofxPostProcessingManager manager;
	ofxDC_Utilities notifier;

	bool hideGui;
	bool automateEffects;
};
