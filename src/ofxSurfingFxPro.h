/*
	TODO:
*/


#pragma once

//--

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingPresets.h"

//#define USE_FX_PRO_ofxSurfingPresetsLite
#ifdef USE_FX_PRO_ofxSurfingPresetsLite
#include "ofxSurfingPresetsLite.h"
#endif

//--

class ofxSurfingFxPro
{

public:

	ofxSurfingFxPro();
	~ofxSurfingFxPro();

	void setup();
	void draw();
	void drawGui();

private:

	ImGuiTreeNodeFlags fg = ImGuiTreeNodeFlags_DefaultOpen;
	bool bExpanded = false;

private:

	void update(ofEventArgs& args);

	void setupParams();
	void startup();
	void setupGui();

public:

	void keyPressed(ofKeyEventArgs & args);
	void keyReleased(ofKeyEventArgs & args);
	void windowResized(ofResizeEventArgs & args);

private:

	void exit();

	void setupGuiStyles();
	void buildHelp();

public:

	void begin();
	void begin(ofCamera& cam);
	void end(bool autoDraw = false);

private:

	ofxPostProcessingManager manager;

	ofxDC_Utilities notifier;

	void drawImGui();
	void drawImGuiMain();
	void drawImGuiToggles();
	void drawImGuiControls();

public:

	ofParameter<bool> bGui{ "FX PRO", true };
	ofParameter<bool> bDebug{ "Debug", false };

	ofParameter<bool> bEnable{ "ENABLE", true };

private:

	ofEventListener listener_bEnable; // just to refresh GUI styles (will fade blink buttons when enabled)

	ofParameter<bool> bGui_Internal{ "Gui Internal", false };
	ofParameter<bool> bGui_Controls{ "CONTROLS", true };
	ofParameter<bool> bGui_Toggles{ "TOGGLES", true };

	ofParameter<void> bRandom{ "RANDOM" };
	ofParameter<float> randomProb;
	ofParameter<bool> bPlayRandoms{ "PLAY RANDOMS", false };
	ofParameter<float> playSpeed;

	ofParameter<bool> bGuiWorkflow{ "GuiWorkflow", true }; // disable tab workflow to improve speed...
	ofParameter<bool> bAutoSave{ "Auto Save", true };

	ofParameterGroup params;

	// Callbacks
	void Changed(ofAbstractParameter& e);
	void Changed_Enablers(ofAbstractParameter& e);

	ofxSurfingGui ui;

#ifdef USE__SURFING_RANDOMIZER__FX_PRO
	ofxSurfingRandomizer randomizer;
#endif

	// Settings
	string path_GLOBAL; // this is to folder all files to avoid mixing with other add-ons data

public:

	void setPathGlobal(string s); // must call before setup. disabled by default

private:

	string path_Params_AppSettings;
	string path_Params_Controls;
	ofParameterGroup params_AppSettings; // -> To store app settings between sessions

	ofParameterGroup params_Preset; // -> We queue params to here!

public:

	ofxSurfingPresets presetsManager; // -> Presets Manager. To handle presets for the toggles!
#ifdef USE_FX_PRO_ofxSurfingPresetsLite
	ofxSurfingPresetsLite presetsManagerLite; // -> Presets Manager for all fx controls! (independent of the toggles)
#endif

	//----

public:

	void doPowerFX(int postId, bool bState);
	void doToggleFX(int postId);

	void doRandomFX(int postId, float prob = 0.5f); // the prob of toggle being true (false by default)
	void doRandomFXAll(float prob = 0.5f); // the prob of toggle being true (false by default)
};
