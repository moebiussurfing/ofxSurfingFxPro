#include "ofxSurfingFxPro.h"

//--------------------------------------------------------------
ofxSurfingFxPro::ofxSurfingFxPro()
{
	path_GLOBAL = "ofxSurfingFxPro/"; // this is to folder all files to avoid mixing with other add-ons data
	path_Params_AppSettings = "FxPro_Settings.xml";

	setup();

	ofSetLogLevel("ofxPostProcessingManager", OF_LOG_ERROR);
}

//--------------------------------------------------------------
ofxSurfingFxPro::~ofxSurfingFxPro()
{
	exit();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;

	ofxSurfingHelpers::CheckFolder(path_GLOBAL);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setupGui()
{
	guiManager.setWindowsMode(IM_GUI_MODE_WINDOWS_SPECIAL_ORGANIZER);
	guiManager.setName("FxPro");
	guiManager.setup();

	guiManager.addWindowSpecial(bGui);
	guiManager.addWindowSpecial(bGui_Toggles);
	guiManager.addWindowSpecial(bGui_Controls);
	guiManager.addWindowSpecial(presetsManager.bGui);

	guiManager.startup();

	bKeys_FX.makeReferenceTo(guiManager.bKeys);

	//--

	refreshStyles();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setupParams()
{
	// Callbacks
	params.setName("Callbacks");
	params.add(bRandom);

	ofAddListener(params.parameterChangedE(), this, &ofxSurfingFxPro::Changed);

	//--

	probFX.set("Prob", 0.5, 0, 1);
	probSpeed.set("Speed", 0.5, 0, 1);

	// Session Settings
	params_AppSettings.setName("FX PRO");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_Controls);
	params_AppSettings.add(bGui_Toggles);
	params_AppSettings.add(bGui_Internal);
	params_AppSettings.add(probFX);
	params_AppSettings.add(probSpeed);
	params_AppSettings.add(bAutomate);
	params_AppSettings.add(bDebug);

	// Get notified when toggles changed!
	ofAddListener(manager.params_Toggles.parameterChangedE(), this, &ofxSurfingFxPro::Changed_Enablers);

	//--

	// Presets

	//TODO:
	// fix recursive nested groups

	/*
	params_Preset.setName("FX PRO");
	params_Preset.add(manager.params_Toggles);

	// For performance issues or to reduce preset files sizes, we can exclude these by commenting!
	//params_Preset.add(manager.params_Controls);

	presetsManager.addGroup(params_Preset);
	*/

	presetsManager.addGroup(manager.params_Toggles);

	//TODO:
	// In some scenarios we will prefer to disable this mode.
	presetsManager.setAutoLoadOnReTrig(false);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setup()
{
	ofxSurfingHelpers::setThemeDark_ofxGui();

	// Setup manager
	manager.setup(ofGetWidth(), ofGetHeight());

	//--

	// Params
	setupParams();

	//--

	// Gui
	setupGui();

	//--

	startup();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::startup()
{
	// Load Settings
	//manager.loadSettings();

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);

	setKeyFirstChar('1');
}

//--------------------------------------------------------------
void ofxSurfingFxPro::refreshStyles()
{
	guiManager.clearStyles();

	// Customize all toggles inside the group 
	//guiManager.AddStyleGroupForBools(manager.params_Toggles, OFX_IM_TOGGLE_BORDER_BLINK);
	guiManager.AddStyleGroupForBools(manager.params_Toggles, OFX_IM_TOGGLE_BIG_BORDER_BLINK);

	// Hide groups header
	guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

	//TODO: fix heritate
	//guiManager.AddStyleGroup(manager.params_Controls, OFX_IM_GROUP_HIDDEN_HEADER);

	//--

	/*
	// workflow
	////for (size_t i = 0; i < manager.params_Toggles.size(); i++)

	//ofParameterGroup gc = manager.params_Controls.getGroup();

	cout << "-------------" << endl;

	for (size_t i = 0; i < manager.params_Toggles.size(); i++)
	{
		ofParameter<bool> _b = manager.params_Toggles[i].cast<bool>();
		string n = _b.getName();

		auto& gc = manager.params_Controls.getGroup("Controls");
		auto ss = gc.getGroupHierarchyNames();

		cout << ofToString(ss) << endl;


		//auto& g = manager.params_Controls.getGroup(n);


		//// Group.
		//auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(g);
		//if (parameterGroup)
		//{
		//}


		//if (g)
		//	if (g.getName() == n)
		//	{
		//		if (_b) guiManager.AddStyleGroup(g, OFX_IM_GROUP_DEFAULT);
		//		else guiManager.AddStyleGroup(g, OFX_IM_GROUP_HIDDEN);
		//	}
	}
	*/
	/*
	//for (auto& g : gc)
	for (auto& g : manager.params_Controls)
	{
		//auto& _g = static_cast<ofParameterGroup&>(g);
		//auto& _g = dynamic_cast<ofParameterGroup&>(g);
		//auto& _g = std::dynamic_pointer_cast<ofParameterGroup>(g.newReference());
		auto _g = std::dynamic_pointer_cast<ofParameterGroup>(g);

		bool bEnable = false;

		if (_g)
		{
			cout << _g->getName() << endl;

			//for (auto& b : manager.params_Toggles)
			for (size_t i = 0; i < manager.params_Toggles.size(); i++)
			{
				auto& b = manager.params_Toggles[i];

				ofParameter<bool> _b = b.cast<bool>();

				if (_g->getName() == _b.getName())
				{
					bEnable = true;
					break;
				}

				//auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(b);
				//if (parameterBool)
				//{
				//	ofxImGui::AddParameter(*parameterBool);
				//	continue;
				//}
			}

			if (bEnable)
			{
				guiManager.AddStyleGroup(*_g, OFX_IM_GROUP_DEFAULT);
			}
			else
			{
				guiManager.AddStyleGroup(*_g, OFX_IM_GROUP_HIDDEN);

			}
		}
	}
	*/

	//guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

	ImGuiTreeNodeFlags fg = ImGuiTreeNodeFlags_DefaultOpen;

	SurfingImGuiTypesGroups tON = OFX_IM_GROUP_COLLAPSED;
	SurfingImGuiTypesGroups tOFF = OFX_IM_GROUP_HIDDEN;

	guiManager.AddStyleGroup(manager.gFxaaGroup, (manager.bEnablers[0] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gBloomGroup, (manager.bEnablers[1] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gKaliGroup, (manager.bEnablers[2] ? tON : tOFF), fg);

	guiManager.AddStyleGroup(manager.gGodRaysGroup, (manager.bEnablers[5] ? tON : tOFF), fg);

	guiManager.AddStyleGroup(manager.gSsaoGroup, (manager.bEnablers[7] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gZoomBlurGroup, (manager.bEnablers[8] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gRGBGroup, (manager.bEnablers[9] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gFilmGrainGroup, (manager.bEnablers[10] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gDotScreenGroup, (manager.bEnablers[11] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gGlicthGroup, (manager.bEnablers[12] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gBadTVGroup, (manager.bEnablers[13] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gcolorACESGroup, (manager.bEnablers[14] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gNoiseGroup, (manager.bEnablers[15] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gTiltShiftGroup, (manager.bEnablers[16] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gSupGroup, (manager.bEnablers[17] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gGliAutoGroup, (manager.bEnablers[18] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gSpaceColorGroup, (manager.bEnablers[19] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gDitherGroup, (manager.bEnablers[20] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gStrobberGroup, (manager.bEnablers[21] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gRimbLightGroup, (manager.bEnablers[22] ? tON : tOFF), fg);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::update()
{
	if (bAutomate)
	{
		float v = ofMap(probSpeed, 0, 1, 0.1, 4);
		if (notifier.notifyPerSecond(v))
		{
			doRandomFXAll(probFX);

			//manager.doEnableNone();
			//manager.doToggleFX(int(ofRandom(manager.getAmountEffects())));
		}
	}

	if (presetsManager.isRetrigged()) {

	}

	manager.updateFX();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawGui() {

	if (!bGui) return;

	drawImGui();

	if (bGui_Internal) manager.drawGui();
	if (bDebug) manager.drawDebug();

	presetsManager.draw(); // Draw Gui
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiMain()
{
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

	//if (guiManager.beginWindow(bGui))
	if (guiManager.beginWindowSpecial(bGui))
	{
		guiManager.AddLabelHuge("FX PRO");
		guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
		guiManager.Add(bKeys_FX, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
		guiManager.AddSpacingBigSeparated();

		guiManager.AddLabelBig("Panels", true, true);
		guiManager.Add(bGui_Toggles, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.Add(bGui_Controls, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.AddSpacing();
		guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.AddSpacingSeparated();

		if (!guiManager.bMinimize)
		{
			guiManager.Add(guiManager.getGuiToggleOrganizer(), OFX_IM_TOGGLE_ROUNDED);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Helpers", true, true);
			guiManager.Add(manager.bNone, OFX_IM_BUTTON, 2, true);
			guiManager.Add(manager.bAll, OFX_IM_BUTTON, 2);
			guiManager.AddSpacingSeparated();

			guiManager.AddLabelBig("Randomizers", true, true);
			guiManager.Add(probFX);
			guiManager.Add(bRandom, OFX_IM_BUTTON);
			guiManager.Add(probSpeed);
			guiManager.Add(bAutomate, OFX_IM_TOGGLE_BORDER_BLINK);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Settings", true, true);
			guiManager.Add(manager.btnLoad);
			guiManager.Add(manager.btnSave);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Debug", true, true);
			guiManager.Add(bGui_Internal, OFX_IM_TOGGLE_ROUNDED_MINI);
			guiManager.Add(bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);
		}

		guiManager.AddLabelBig("WORKFLOW", true, true);
		guiManager.Add(bGuiWorkflow, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

		//guiManager.endWindow();
		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiControls()
{
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_MEDIUM;

	if (guiManager.beginWindowSpecial(bGui_Controls))
	{
		guiManager.AddGroup(manager.params_Controls);

		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiToggles()
{
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

	if (guiManager.beginWindowSpecial(bGui_Toggles))
	{
		guiManager.AddGroup(manager.params_Toggles);

		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGui()
{
	guiManager.begin();
	{
		//--

		// Main
		drawImGuiMain();

		//--

		// Toggles 
		drawImGuiToggles();

		//--

		// Controls
		drawImGuiControls();
	}
	guiManager.end();
}

//---------------------------------------
void ofxSurfingFxPro::begin() {
	manager.begin();
}
//---------------------------------------
void ofxSurfingFxPro::begin(ofCamera& cam) {
	manager.begin(cam);
}
//---------------------------------------
void ofxSurfingFxPro::end() {
	manager.end();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::keyPressed(int key)
{
	if (bKeys_FX) keyPressedFX(key);

	if (key == OF_KEY_BACKSPACE) manager.doEnableNone();
	if (key == OF_KEY_RETURN) doRandomFXAll(probFX);

	//else if (key == '1') manager.loadSettings("1.json");
	//else if (key == '2') manager.loadSettings("2.json");
	//else if (key == '3') manager.loadSettings("3.json");

	//else if (key == 'g') bGui = !bGui;
	//else if (key == 'r') doRandomPickOne();
	//else if (key == 'a') bAutomate = !bAutomate;
}

//--------------------------------------------------------------
void ofxSurfingFxPro::windowResized(int w, int h) {
	manager.windowResized(w, h);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::exit()
{
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);

	ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingFxPro::Changed);

	ofRemoveListener(manager.params_Toggles.parameterChangedE(), this, &ofxSurfingFxPro::Changed_Enablers);
}

////--------------------------------------------------------------
//void ofxSurfingFxPro::doRandomPickOne() {
//	manager.doEnableNone();
//	int randId = int(ofRandom(manager.getAmountEffects()));
//	manager.doToggleFX(randId);
//}

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (name == bRandom.getName() && bRandom)
	{
		bRandom = false;

		//doRandomPickOne();
		doRandomFXAll(probFX);
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed_Enablers(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (bGuiWorkflow) refreshStyles();
}

//---

//--------------------------------------------------------------
void ofxSurfingFxPro::keyPressedFX(int key)
{
	char k0 = keyCommandsChars[keyFirstPos];

	for (size_t k = 0; k < manager.getAmountEffects(); k++)
	{
		if (key == keyCommandsChars[k]) { doToggleFX(k); return; }
	}
}

//--------------------------------------------------------
void ofxSurfingFxPro::doToggleFX(int postId)
{
	manager.doToggleFX(postId);
}

//--------------------------------------------------------
void ofxSurfingFxPro::doPowerFX(int postId, bool bState)
{
	manager.doPowerFX(postId, bState);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::doRandomFX(int postId, float prob)
{
	if (postId > manager.getAmountEffects()) return;

	bool b = (bool)(ofRandom(1.0f) < prob);
	manager.doPowerFX(postId, b);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::doRandomFXAll(float prob)
{
	for (int i = 0; i < manager.getAmountEffects(); i++)
	{
		bool b = (bool)(ofRandom(1.0f) < prob);
		manager.doPowerFX(i, b);
	}

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	presetsManager.undoManager.doSaveUndoWhenAuto();
#endif
}
