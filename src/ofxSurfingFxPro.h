#pragma once

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingPresets.h"

class ofxSurfingFxPro {

public:

	ofxSurfingFxPro();
	~ofxSurfingFxPro();

	void setup();
	void update();

	void drawGui();
	void keyPressed(int key);
	void windowResized(int w, int h);
	void exit();
	void refreshStyles();

	void begin();
	void begin(ofCamera& cam);
	void end();

	ofxPostProcessingManager manager;
	ofxDC_Utilities notifier;

	void doRandom();
	
	void drawImGui();
	void drawImGuiMain();
	void drawImGuiToggles();
	void drawImGuiControls();

	ofParameter<bool> bGui{ "FxPro", true};
	ofParameter<bool> bGui_Internal{ "Gui Internal", false };
	ofParameter<bool> bGui_Controls{ "CONTROLS", true};
	ofParameter<bool> bGui_Toggles{ "TOGGLES", true};

	ofParameter<bool> bRandom{ "Random", true };
	ofParameter<bool> bAutomate{ "Automate", false};
	ofParameter<bool> bDebug { "Debug", false };
	
	ofParameterGroup params;
	void Changed(ofAbstractParameter& e);

	void Changed_Enablers(ofAbstractParameter& e);

	ofxSurfing_ImGui_Manager guiManager;

	// settings
	string path_GLOBAL; // this is to folder all files to avoid mixing with other addons data
	string path_Params_AppSettings;
	void setPathGlobal(string s); // must call before setup. disabled by default
	ofParameterGroup params_AppSettings;

	//ofParameterGroup& getParams() {
	//	manager.params;
	//}

	ofxSurfingPresets presetsManager;
};
