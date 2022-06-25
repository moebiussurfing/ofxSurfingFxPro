#pragma once

#include "ofMain.h"

#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingPresets.h"

class ofxSurfingFxPro
{

public:

	ofxSurfingFxPro();
	~ofxSurfingFxPro();

	void setup();
	void setupParams();
	void startup();
	void setupGui();
	void update();

	void drawGui();
	void keyPressed(int key);
	void keyPressedFX(int key);
	void windowResized(int w, int h);
	void exit();
	void refreshStyles();

	void begin();
	void begin(ofCamera& cam);
	void end();

	ofxPostProcessingManager manager;
	ofxDC_Utilities notifier;

	void doRandomPickOne();

	void drawImGui();
	void drawImGuiMain();
	void drawImGuiToggles();
	void drawImGuiControls();

	ofParameter<bool> bGui{ "FX PRO", true };
	ofParameter<bool> bGui_Internal{ "Gui Internal", false };
	ofParameter<bool> bGui_Controls{ "CONTROLS", true };
	ofParameter<bool> bGui_Toggles{ "TOGGLES", true };

	ofParameter<bool> bRandom{ "Random", true };
	ofParameter<bool> bAutomate{ "Automated", false };
	ofParameter<bool> bDebug{ "Debug", false };
	ofParameter<bool> bGuiWorkflow{ "GuiWorkflow", true }; // disable tab workflow to improve speed...

	ofParameter<float> probFX;
	ofParameter<float> probSpeed;

	ofParameter<bool> bKeys_FX;
	ofParameter<bool> bKeys_ModeToggle;

	ofParameterGroup params;

	// Callbacks
	void Changed(ofAbstractParameter& e);
	void Changed_Enablers(ofAbstractParameter& e);

	ofxSurfing_ImGui_Manager guiManager;

	// Settings
	string path_GLOBAL; // this is to folder all files to avoid mixing with other add-ons data
	void setPathGlobal(string s); // must call before setup. disabled by default
	string path_Params_AppSettings;
	ofParameterGroup params_AppSettings; // -> To store app settings between sessions

	////--------------------------------------------------------------
	//ofParameterGroup& getParams() {
	//	manager.params;
	//}

	ofParameterGroup params_Preset; // -> We queue params to here!
	ofxSurfingPresets presetsManager; // -> To handle presets.

	//----

	void doPowerFX(int postId, bool bState);
	void doToggleFX(int postId);

	void doRandomFX(int postId, float prob = 0.5f); // the prob of toggle being true (false by default)
	void doRandomFXAll(float prob = 0.5f); // the prob of toggle being true (false by default)

	//----

	// Key Commands
	
	// These methods allows to customize key commands assignments 
	// In this case to trig the FX toggles.

private:

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

	vector<char> keyCommandsChars; // Prepared chars used as triggers

public:

	// Customizable Key commands
	// '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	// 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	// 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	// 'z', 'x', 'c', 'v', 'b', 'n', 'm'

	// Customizable keys to avoid collide with other key commands callbacks
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
		
};
