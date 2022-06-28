#include "ofxSurfingFxPro.h"

//--------------------------------------------------------------
ofxSurfingFxPro::ofxSurfingFxPro()
{
	path_GLOBAL = "ofxSurfingFxPro/"; // this is to folder all files to avoid mixing with other add-ons data
	path_Params_AppSettings = "FxPro_Settings.xml";

	setup();

	//--
	// 
	//TODO: fix log silencers
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

	//bKeys_FX.makeReferenceTo(guiManager.bKeys);

	//--

	setupGuiStyles();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setupParams()
{
	// Callbacks
	params.setName("Callbacks");
	params.add(bRandom);

	ofAddListener(params.parameterChangedE(), this, &ofxSurfingFxPro::Changed);

	//--

	randomProb.set("Prob", 0.5f, 0.05f, 1);
	playSpeed.set("Speed", 0.5f, 0, 1);
	bKeys_FX.set("KEYS FX", true);
	bKeys_FX_ToggleMode.set("MODE TOGGLE", false);

	// Session Settings
	params_AppSettings.setName("FX PRO");

	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_Controls);
	params_AppSettings.add(bGui_Toggles);
	params_AppSettings.add(bGui_Internal);
	params_AppSettings.add(bKeys_FX);
	params_AppSettings.add(bKeys_FX_ToggleMode);
	params_AppSettings.add(bEnable);
	params_AppSettings.add(playSpeed);
	params_AppSettings.add(bPlayRandoms);
	params_AppSettings.add(randomProb);
	//params_AppSettings.add(bDebug);

	// Get notified when toggles changed!
	ofAddListener(manager.params_Toggles.parameterChangedE(), this, &ofxSurfingFxPro::Changed_Enablers);

	listener_bEnable = bEnable.newListener([this](bool&)
		{
			ofLogNotice("ofApp") << "bEnable: " << bEnable;
			setupGuiStyles();
		});

	//--

	// Presets

	/*

	//TODO:
	// fix recursive nested groups

	params_Preset.setName("FX PRO");
	params_Preset.add(manager.params_Toggles);

	// For performance issues or to reduce preset files sizes, we can exclude these by commenting!
	//params_Preset.add(manager.params_Controls);

	presetsManager.addGroup(params_Preset);

	*/

	presetsManager.addGroup(manager.params_Toggles);

	//TODO:
	// In some scenarios we will prefer to disable this feature/mode.
	// That's to avoid auto reload the current file preset again.
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
	// could be removed. it loads all the settings from the internal ofxGui
	//manager.loadSettings();

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);

	//setKeyFirstChar('1');

	setKeyFirstChar('q'); // Default key list starts on '1'. 
	// but would collide with presets manager keys!
	// Then we overwrite to start from q key, and will got until b,
	// bc the amount of FX included on FxPro!

	// Load Control Settings
	// Not the Toggles! 
	// Toggles are handled by the Presets Manager!
	ofxSurfingHelpers::load(manager.params_Controls);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setupGuiStyles()
{
	guiManager.clearStyles();

	// Customize all toggles inside the group 
	guiManager.AddStyleGroupForBools(manager.params_Toggles, bEnable ? OFX_IM_TOGGLE_MEDIUM_BORDER_BLINK : OFX_IM_TOGGLE_MEDIUM);

	// Hide groups header
	guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

	// Controls

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

	// Hide group headers
	//TODO: fix heritate
	//guiManager.AddStyleGroup(manager.params_Controls, OFX_IM_GROUP_HIDDEN_HEADER);
	//guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

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
}

//--------------------------------------------------------------
void ofxSurfingFxPro::update()
{
	if (bPlayRandoms)
	{
		float v = ofMap(playSpeed, 1, 0, 0.2f, 2.f);
		if (notifier.notifyPerSecond(v))
		{
			doRandomFXAll(randomProb);

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

	//notifier.drawFPS(ofxDC_ALIGNMENT::TOP_RIGHT);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiMain()
{
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

	//if (guiManager.beginWindow(bGui))
	if (guiManager.beginWindowSpecial(bGui))
	{
		guiManager.AddLabelHuge("FX PRO");
		guiManager.Add(bEnable, OFX_IM_TOGGLE_BIG_XXL_BORDER);
		guiManager.AddSpacingBigSeparated();

		guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
		guiManager.Add(bKeys_FX, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

		if (bKeys_FX) {
			string s = string("Key controls goes from ") + getFirstKey() + string(" to ") + getLastKey();
			guiManager.AddTooltip(s);

			guiManager.Indent();
			guiManager.Add(bKeys_FX_ToggleMode, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			if (!bKeys_FX_ToggleMode) guiManager.AddTooltip("Press SHIFT before release key to latch");
			guiManager.Unindent();
		}
		guiManager.AddSpacingBigSeparated();

		guiManager.AddLabelBig("Panels"/*, true, true*/);
		guiManager.Add(bGui_Toggles, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.Add(bGui_Controls, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.AddSpacingSeparated();

		guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);

		if (!guiManager.bMinimize)
		{
			guiManager.AddSpacingSeparated();

			guiManager.AddLabelBig("Control Settings"/*, true, true*/);
			if (guiManager.AddButton("SAVE", OFX_IM_BUTTON_MEDIUM, 2))
			{
				ofxSurfingHelpers::save(manager.params_Controls);
			}
			guiManager.AddTooltip("Save Controls. \nIndependently of Toggle states, \nhandled by Presets");
			guiManager.SameLine();
			if (guiManager.AddButton("LOAD", OFX_IM_BUTTON_MEDIUM, 2))
			{
				ofxSurfingHelpers::load(manager.params_Controls);
			}
			guiManager.AddTooltip("Load Controls. \nIndependently of Toggle states, \nhandled by Presets");
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Helpers"/*, true, true*/);
			guiManager.Add(manager.bNone, OFX_IM_BUTTON, 2, true);
			guiManager.Add(manager.bAll, OFX_IM_BUTTON, 2);
			guiManager.AddSpacingSeparated();

			static bool bOpen = false;
			ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
			if (ImGui::CollapsingHeader("RANDOMIZERS", _flagw))
			{
				guiManager.refreshLayout();
				//guiManager.AddLabelBig("Randomizers", true, true);
				guiManager.Add(randomProb);
				guiManager.Add(bRandom, OFX_IM_BUTTON_MEDIUM);
				guiManager.AddTooltip("Randomize params but using Prob: \nbigger is too much toggles will be ON");

				guiManager.Add(bPlayRandoms, OFX_IM_TOGGLE_MEDIUM_BORDER_BLINK);
				if (bPlayRandoms) {
					guiManager.Add(playSpeed);
					guiManager.AddTooltip("Faster is 4 random trigs per second. \nSlower is one for each passed 2 seconds.");
				}
			}
			guiManager.refreshLayout();
			guiManager.AddSpacingSeparated();

			//// not required
			//guiManager.AddLabelBig("Debug", true, true);
			//guiManager.Add(bGui_Internal, OFX_IM_TOGGLE_ROUNDED_MINI);
			//guiManager.Add(bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);

			guiManager.AddLabelBig("WORKFLOW"/*, true, true*/);
			guiManager.Add(bGuiWorkflow, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			guiManager.Add(guiManager.getGuiToggleOrganizer(), OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		}

		// Help
		guiManager.AddSpacingBigSeparated();
		guiManager.Add(guiManager.bHelp, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		//guiManager.Add(guiManager.bDebug, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

		//guiManager.endWindow();
		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiControls()
{
	//IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

	// Constraint Window Shape
	float w = 250;
	ImVec2 size_min = ImVec2(w, -1);
	ImVec2 size_max = ImVec2(w + 0, -1);
	ImGui::SetNextWindowSizeConstraints(size_min, size_max);

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
	if (bEnable) manager.begin();
}
//---------------------------------------
void ofxSurfingFxPro::begin(ofCamera& cam) {//begin to draw scene inside to process!
	if (bEnable) manager.begin(cam);
	else cam.begin();
}
//---------------------------------------
void ofxSurfingFxPro::end() {//ends the drawn scene and draws processed image!
	if (bEnable) manager.end();
	//else cam.end();//why not required to end cam?
}

//--------------------------------------------------------------
void ofxSurfingFxPro::keyPressed(int key)
{
	if (bKeys_FX)
	{
		keyPressedFX(key);
		if (key == OF_KEY_LEFT_SHIFT) bShiftPressed = true;
	}

	//--

	if (key == OF_KEY_F9) manager.doEnableNone();
	if (key == OF_KEY_F9) doRandomFXAll(randomProb);

	//else if (key == 'G') bGui = !bGui;
}

//--------------------------------------------------------------
void ofxSurfingFxPro::keyReleased(int key)
{
	if (bKeys_FX)
	{
		keyReleasedFX(key);
		if (key == OF_KEY_LEFT_SHIFT) bShiftPressed = false;
	}
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

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (name == bRandom.getName())
	{
		doRandomFXAll(randomProb);
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed_Enablers(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (bGuiWorkflow) setupGuiStyles();
}

//--

//--------------------------------------------------------------
void ofxSurfingFxPro::keyPressedFX(int key)
{
	char k0 = keyCommandsChars[keyFirstPos];

	for (size_t k = 0; k < manager.getAmountEffects(); k++)
	{
		if (key == keyCommandsChars[k])
		{
			// workflow
			// press SHIFT before release the key
			// to maintain the fx toggle on!
			if (bKeys_FX_ToggleMode) doToggleFX(k);
			else doPowerFX(k, true);

			return;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::keyReleasedFX(int key)
{
	if (bKeys_FX_ToggleMode) return;

	char k0 = keyCommandsChars[keyFirstPos];

	for (size_t k = 0; k < manager.getAmountEffects(); k++)
	{
		if (key == keyCommandsChars[k])
		{
			if (!bKeys_FX_ToggleMode) if (!bShiftPressed || !bGuiWorkflow) doPowerFX(k, false);

			return;
		}
	}
}

//--

// Set / Toggle FX enablers

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

// Random FX enablers

//--------------------------------------------------------------
void ofxSurfingFxPro::doRandomFX(int postId, float prob)
{
	if (postId > manager.getAmountEffects()) return;

	manager.doPowerFX(postId, (bool)(ofRandom(1.0f) < prob));
}

//--------------------------------------------------------------
void ofxSurfingFxPro::doRandomFXAll(float prob)
{
	for (int i = 0; i < manager.getAmountEffects(); i++)
	{
		manager.doPowerFX(i, (bool)(ofRandom(1.0f) < prob));
	}

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	presetsManager.undoManager.doSaveUndoWhenAuto();
#endif
}
