#pragma once

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingImGui.h"

class ofxSurfingFxPro {

public:

	ofxSurfingFxPro();
	~ofxSurfingFxPro();

	void setup();
	void update();
	void drawImGui();
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

	//ofParameterGroup& getParams() {
	//	manager.params;
	//}

	ofParameter<bool> bGui{ "Show Gui", true };
	ofxSurfing_ImGui_Manager guiManager;
};
