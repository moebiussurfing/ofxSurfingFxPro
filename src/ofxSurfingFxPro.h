/*

	TODO:

	add directive to disable randomizer


*/


#pragma once

//--

// Optional

//#define USE__ofxSurfingFxPro__ofxSurfingFxPro
#define INCLUDE__OFX_UNDO_ENGINE // -> Comment to disable feature and his dependency

//--

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingPresets.h"

#ifdef USE__ofxSurfingFxPro__ofxSurfingFxPro
#include "ofxSurfingRandomizer.h"
#endif

#ifdef INCLUDE__OFX_UNDO_ENGINE
#include "ofxSurfingUndoHelper.h"
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

	void keyPressed(int key);
	void keyReleased(int key);

private:

	void keyPressedFX(int key);
	void keyReleasedFX(int key);
	bool bShiftPressed = false;

public:

	void windowResized(int w, int h);

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

	ofParameter<bool> bKeys_FX;
	ofParameter<bool> bKeys_FX_ToggleMode;
	ofParameter<bool> bGuiWorkflow{ "GuiWorkflow", true }; // disable tab workflow to improve speed...
	ofParameter<bool> bAutoSave{ "Auto Save", true };

	ofParameterGroup params;

	// Callbacks
	void Changed(ofAbstractParameter& e);
	void Changed_Enablers(ofAbstractParameter& e);

	ofxSurfingGui ui;

#ifdef USE__ofxSurfingFxPro__ofxSurfingFxPro
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

	////--------------------------------------------------------------
	//ofParameterGroup& getParams() {
	//	manager.params;
	//}

	ofParameterGroup params_Preset; // -> We queue params to here!

public:

	ofxSurfingPresets presetsManager; // -> To handle presets.

	//----

public:

	void doPowerFX(int postId, bool bState);
	void doToggleFX(int postId);

	void doRandomFX(int postId, float prob = 0.5f); // the prob of toggle being true (false by default)
	void doRandomFXAll(float prob = 0.5f); // the prob of toggle being true (false by default)

	//----

private:

	// Key Commands

	// These methods allows to customize key commands assignments 
	// In this case to trig the FX toggles.

#define NUM_KEY_COMMANDS 36

	// This is the sorted map of keys:
	// We will select the starting key, then will follow next ones to assign to next preset index command!
	// Then we avoid to collide keycommands when using multiple presets manager instances 
	// or when colliding with other add-ons.

	// Predefined picked keys to assign commands
	char keysFullMap[NUM_KEY_COMMANDS] = {
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	'z', 'x', 'c', 'v', 'b', 'n', 'm' };

	int keyFirstPos = -1;
	char keyFirstChar = '0';

	vector<char> keyCommandsChars; // Prepared chars that will be used as triggers.

public:

	// Default Keys
	// '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	// 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	// 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	// 'z', 'x', 'c', 'v', 'b', 'n', 'm'

	// Customizable keys to avoid collide with other key commands callbacks
	// We set the first, next ones are correlative from the above default keys!
	//--------------------------------------------------------------
	void setKeyFirstChar(char kChar) {
		int p = getKeyCommandPosition(kChar);
		if (p == -1)
		{
			ofLogError(__FUNCTION__) << "Can't found key: " << kChar;
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "Set key first command (to correlative keys starting) at key: " << kChar;

			setKeyFirstPos(p);

			keyCommandsChars.clear();
			for (size_t i = p; i < NUM_KEY_COMMANDS; i++)
			{
				keyCommandsChars.push_back(keysFullMap[i]);
			}
		}
	}

private:

	//--------------------------------------------------------------
	void setKeyFirstPos(int kPos)
	{
		ofLogNotice(__FUNCTION__) << "Set key first position at index: " << kPos;

		keyFirstPos = kPos;
	}

	//--------------------------------------------------------------
	int getKeyCommandPosition(char key) {
		int pos = -1;
		for (size_t i = 0; i < NUM_KEY_COMMANDS; i++)
		{
			if (key == keysFullMap[i])
			{
				pos = i;
				return pos;
			}
		}
		return pos;
	}

	// Mainly for displaying info purposes
	//--------------------------------------------------------------
	string getFirstKey() {
		if (keyCommandsChars.size() != 0) return ofToString(keyCommandsChars[0]);
		else return "-1";
	}
	//--------------------------------------------------------------
	string getLastKey() {
		if (keyCommandsChars.size() != 0)
		{
			int amountRequiredKeys = manager.getAmountEffects();

			string s;
			if (amountRequiredKeys < keyCommandsChars.size()) {
				s = keyCommandsChars[amountRequiredKeys - 1];
				return s;
			}
			else {
				// last key is before/less than the last FX
				// we should need more key commands!
				// some FX's don't have a key trigger.
				return ofToString(keyCommandsChars.back());
			}
		}

		else return "-1";
	}


#ifdef INCLUDE__OFX_UNDO_ENGINE
private:

	ofxSurfingUndoHelper undoManager;
	ofParameterGroup params_Undo;
#endif
};
