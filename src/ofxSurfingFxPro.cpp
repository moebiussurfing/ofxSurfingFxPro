#include "ofxSurfingFxPro.h"

//--------------------------------------------------------------
ofxSurfingFxPro::ofxSurfingFxPro()
{
	ofAddListener(ofEvents().update, this, &ofxSurfingFxPro::update);

	path_GLOBAL = "ofxSurfingFxPro/"; // this is to folder all files to avoid mixing with other add-ons data
	path_Params_AppSettings = "FxPro_AppSettings.xml";
	path_Params_Controls = "FxPro_Controls.xml";

	setup();

	//--

	//TODO: fix log silencers
	//ofSetLogLevel("ofxPostProcessingManager", OF_LOG_ERROR);
	//ofSetLogLevel("ofxSurfingFxPro", OF_LOG_SILENT);
	ofSetLogLevel(OF_LOG_SILENT);
}

//--------------------------------------------------------------
ofxSurfingFxPro::~ofxSurfingFxPro()
{
	ofRemoveListener(ofEvents().update, this, &ofxSurfingFxPro::update);

	exit();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setPathGlobal(string s) // must call before setup. disabled by default
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
	//guiManager.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);

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
	params_AppSettings.add(bEnable);
	params_AppSettings.add(bKeys_FX);
	params_AppSettings.add(bKeys_FX_ToggleMode);

	//could be removed instead of randomizer addon
	params_AppSettings.add(playSpeed);
	params_AppSettings.add(bPlayRandoms);
	params_AppSettings.add(randomProb);

	//params_AppSettings.add(randomizer.bGui);
	//params_AppSettings.add(randomizer.bGui_Main);

	//params_AppSettings.add(bAutoSave);//commented bc prefer forced to restart true
	//params_AppSettings.add(bDebug);//commented bc forced to restart disabled

	//--

	// Get notified when any toggle changed!
	ofAddListener(manager.params_Toggles.parameterChangedE(), this, &ofxSurfingFxPro::Changed_Enablers);

	listener_bEnable = bEnable.newListener([this](bool&)
		{
			ofLogNotice("ofApp") << "bEnable: " << bEnable;
			setupGuiStyles();
		});

	//----

	// Presets Manager

	/*

	//TODO:
	// fix recursive nested groups

	params_Preset.setName("FX PRO");
	params_Preset.add(manager.params_Toggles);

	// For performance issues or to reduce preset files sizes, we can exclude these by commenting!
	//params_Preset.add(manager.params_Controls);

	presetsManager.addGroup(params_Preset);

	*/

	//presetsManager.setDiablePlayer();//simplify bc we have the randomizer player

	presetsManager.addGroup(manager.params_Toggles);

	//TODO:
	// In some scenarios we will prefer to disable this feature/mode.
	// That's to avoid auto reload the current file preset again.
	presetsManager.setAutoLoadOnReTrig(false);

	//--

	// Randomizer

	randomizer.setup(manager.params_Controls);
	//randomizer.setup(manager.params_Toggles);

	randomizer.setIndexPtr(presetsManager.index);

	//--

	// Target B. 
	// Index

	// Link index with the Presets Manager selector!
	randomizer.setIndexPtr(presetsManager.index);

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
	if (bAutoSave) ofxSurfingHelpers::loadGroup(manager.params_Controls, path_GLOBAL + path_Params_Controls);

	//--

	// Force not both keys callbacks enabled to avoid collide!
	//presetsManager.bKeys = !randomizer.bKeys;

	// Create some presets
	//presetsManager.doPopulatePresetsRandomized();

	//TODO: fix
	// Force visible on first start 
	//randomizer.setGuiVisible(true);
	//presetsManager.setGuiVisible(true);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::buildHelp()
{
	// Help Info
	{
		std::string helpInfo = "";

		helpInfo += "FxPro \n";
		helpInfo += "HELP \n";
		helpInfo += "\n";
		helpInfo += "KEY COMMANDS \n";
		helpInfo += "\n";

		helpInfo += "G                GUI \n";
		helpInfo += "H                HELP APP \n";
		helpInfo += "\n";

		helpInfo += "PANELS \n";
		helpInfo += "F1               TOGGLES \n";
		helpInfo += "F2               CONTROLS \n";
		helpInfo += "F3               PRESETS \n";
		helpInfo += "F4               RANDOMIZER \n";
		helpInfo += "\n";

		helpInfo += "FX \n";
		if (!bKeys_FX)
		{
			helpInfo += "KEYS FX toggle is disabled. \n";
			helpInfo += "Enable that toggle! \n";
		}
		else {
			helpInfo += "q to v           FX TOGGLES \n";
			helpInfo += "SHIFT            Latch on MODE TOGGLE \n";
		}
		helpInfo += "\n";

		helpInfo += "PRESETS \n";
		if (!presetsManager.bKeys)
		{
			helpInfo += "KEYS toggle is disabled. \n";
			helpInfo += "Enable that toggle! \n";
		}
		else {
			helpInfo += "1 to 9           BROWSE \n";
			helpInfo += "< >              \n";
			helpInfo += "SPACE            NEXT \n";
			helpInfo += "+Ctrl            PLAY \n";
		}
		helpInfo += "\n";

		helpInfo += "NOTE \n";
		helpInfo += "Take care when enabling many KEYS toggles \n";
		helpInfo += "from different add-ons at the same time. \n";
		helpInfo += "Key commands could collide! \n";

		guiManager.setHelpInfoApp(helpInfo);
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setupGuiStyles()
{
	buildHelp();

	//--

	guiManager.clearStyles();

	// Customize all toggles inside the group 
	guiManager.AddStyleGroupForBools(manager.params_Toggles, bEnable ? OFX_IM_TOGGLE_MEDIUM_BORDER_BLINK : OFX_IM_TOGGLE_MEDIUM);

	// Hide groups header
	guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

	//--

	// Controls

	//ImGuiTreeNodeFlags fg = ImGuiTreeNodeFlags_DefaultOpen;

	SurfingImGuiTypesGroups tON = (bExpanded ? OFX_IM_GROUP_TREE_EX : OFX_IM_GROUP_COLLAPSED);
	//SurfingImGuiTypesGroups tON = OFX_IM_GROUP_COLLAPSED;

	SurfingImGuiTypesGroups tOFF = OFX_IM_GROUP_HIDDEN;

	guiManager.AddStyleGroup(manager.gFxaaGroup, (manager.bEnablers[0] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gBloomGroup, (manager.bEnablers[1] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gKaliGroup, (manager.bEnablers[2] ? tON : tOFF), fg);
	// #3, #4 // don't have params
	guiManager.AddStyleGroup(manager.gGodRaysGroup, (manager.bEnablers[5] ? tON : tOFF), fg);
	// #6 // don't have params
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

	//--

	//// Hide group headers
	////TODO: fix heritate
	//guiManager.AddStyleGroup(manager.params_Controls, OFX_IM_GROUP_HIDDEN_HEADER);
	////guiManager.AddStyleGroup(manager.params_Toggles, OFX_IM_GROUP_HIDDEN_HEADER);

	//--

	// Update the queued params to randomize

	static ofParameterGroup g;
	g.clear();
	g.setName("Params");

	if (manager.bEnablers[0]) g.add(manager.gFxaaGroup);
	if (manager.bEnablers[1]) g.add(manager.gBloomGroup);
	if (manager.bEnablers[2]) g.add(manager.gKaliGroup);
	// #3, #4
	if (manager.bEnablers[5]) g.add(manager.gGodRaysGroup);
	// #6
	if (manager.bEnablers[7]) g.add(manager.gSsaoGroup);
	if (manager.bEnablers[8]) g.add(manager.gZoomBlurGroup);
	if (manager.bEnablers[9]) g.add(manager.gRGBGroup);
	if (manager.bEnablers[10]) g.add(manager.gFilmGrainGroup);
	if (manager.bEnablers[11]) g.add(manager.gDotScreenGroup);
	if (manager.bEnablers[12]) g.add(manager.gGlicthGroup);
	if (manager.bEnablers[13]) g.add(manager.gBadTVGroup);
	if (manager.bEnablers[14]) g.add(manager.gcolorACESGroup);
	if (manager.bEnablers[15]) g.add(manager.gNoiseGroup);
	if (manager.bEnablers[16]) g.add(manager.gTiltShiftGroup);
	if (manager.bEnablers[17]) g.add(manager.gSupGroup);
	if (manager.bEnablers[18]) g.add(manager.gGliAutoGroup);
	if (manager.bEnablers[19]) g.add(manager.gSpaceColorGroup);
	if (manager.bEnablers[20]) g.add(manager.gDitherGroup);
	if (manager.bEnablers[21]) g.add(manager.gStrobberGroup);
	if (manager.bEnablers[22]) g.add(manager.gRimbLightGroup);

	randomizer.rebuildParamsGroup(g);

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
//void ofxSurfingFxPro::update()
void ofxSurfingFxPro::update(ofEventArgs& args)
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

	//--

	//TODO:
	// Undo Engine
	if (randomizer.isRandomized())
	{
		// Presets only handles toggles!
//#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
//		presetsManager.undoManager.doSaveUndoWhenAuto();
//#endif
	}

	//--

	//TODO:
	if (presetsManager.isRetrigged())
	{

	}

	//--

	if (bEnable) manager.updateFX();

	//--

	// Update help info when keys toggle changed
	// TODO: should link all sections / add-ons
	// easy callback

	bool bUpdate = false;
	static bool bKeys_FX_ = !bKeys_FX;
	static bool presetsManagerbKeys_ = !presetsManager.bKeys;
	if (bKeys_FX_ != bKeys_FX) {
		bKeys_FX_ = bKeys_FX;
		bUpdate = true;
	}
	if (presetsManagerbKeys_ != presetsManager.bKeys) {
		presetsManagerbKeys_ = presetsManager.bKeys;
		bUpdate = true;
	}
	
	if(bUpdate) buildHelp();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::draw()
{
	manager.draw(bEnable);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawGui() {

	if (!bGui) return;

	drawImGui();

	if (bGui_Internal) manager.drawGui(); // original ofxGui 

	if (bDebug)
	{
		manager.drawDebug();
		notifier.drawFPS(ofxDC_ALIGNMENT::TOP_RIGHT);
	}

	presetsManager.drawGui();

	randomizer.drawGui();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiMain()
{
	//IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

	//if (guiManager.beginWindow(bGui))
	if (guiManager.beginWindowSpecial(bGui))
	{
		guiManager.AddLabelHuge("FX PRO");
		guiManager.Add(bEnable, OFX_IM_TOGGLE_BIG_BORDER);
		guiManager.AddSpacingBigSeparated();

		guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
		guiManager.Add(bKeys_FX, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

		if (bKeys_FX)
		{
			string s = string("Key controls goes from ") + getFirstKey() + string(" to ") + getLastKey();
			guiManager.AddTooltip(s);

			guiManager.Indent();
			guiManager.Add(bKeys_FX_ToggleMode, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			if (!bKeys_FX_ToggleMode) guiManager.AddTooltip("Press SHIFT before release key to latch");
			guiManager.Unindent();
		}
		guiManager.AddSpacingBigSeparated();

		//--

		guiManager.AddLabelBig("Panels");

		// Toggles
		guiManager.Add(bGui_Toggles, OFX_IM_TOGGLE_ROUNDED_BIG);

		// Controls
		guiManager.Indent();
		guiManager.Add(bGui_Controls, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
		guiManager.Unindent();


		// Presets Manager
		guiManager.AddSpacingSeparated();
		guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_ROUNDED_BIG);
		//if (!presetsManager.bGui) {
		//	guiManager.Indent();
		//	presetsManager.draw_ImGui_ClickerSimple(true, false, true, false);
		//	guiManager.Unindent();
		//}
		guiManager.AddSpacingSeparated();

		// Randomizer
		guiManager.Add(randomizer.bGui, OFX_IM_TOGGLE_ROUNDED_BIG);

		//--

		if (!guiManager.bMinimize)
		{
			if (bGui_Controls) {
				guiManager.AddSpacingSeparated();

				guiManager.AddLabelBig("Controls"/*, true, true*/);
				if (guiManager.AddButton("SAVE", OFX_IM_BUTTON_MEDIUM, 2))
				{
					ofxSurfingHelpers::save(manager.params_Controls);
				}
				guiManager.AddTooltip("Save Controls. \nHandled independently of the Toggle states, \nthat are handled by Presets Manager.");
				guiManager.SameLine();
				if (guiManager.AddButton("LOAD", OFX_IM_BUTTON_MEDIUM, 2))
				{
					ofxSurfingHelpers::load(manager.params_Controls);
				}
				guiManager.AddTooltip("Load Controls. \nHandled independently of the Toggle states, \nthat are handled by Presets Manager.");
				guiManager.Add(bAutoSave);
				guiManager.AddTooltip("Auto Store and Recall Controls Settings on the next App session.\nExcept for Toggles, that are handled by the Presets Manager!");
			}

			guiManager.AddSpacingBigSeparated();

			//--

			// Extra

			guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED);
			if (guiManager.bExtra)
			{
				guiManager.Indent();

				//guiManager.AddLabelBig("Helpers"/*, true, true*/);
				//guiManager.Add(manager.bNone, OFX_IM_BUTTON, 2, true);
				//guiManager.Add(manager.bAll, OFX_IM_BUTTON, 2);
				//guiManager.AddSpacingSeparated();

				static bool bOpen = false;
				ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("RANDOMIZERS", _flagw))
				{
					guiManager.refreshLayout();
					//guiManager.AddLabelBig("Randomizers", true, true);
					guiManager.Add(randomProb);
					guiManager.Add(bRandom, OFX_IM_BUTTON_MEDIUM);
					guiManager.AddTooltip("Randomize params but using Prob: \nWhen bigger too much toggles will be ON");

					guiManager.Add(bPlayRandoms, OFX_IM_TOGGLE_MEDIUM_BORDER_BLINK);
					if (bPlayRandoms) {
						guiManager.Add(playSpeed);
						guiManager.AddTooltip("Faster is 4 random trigs per second. \nSlower is one for each passed 2 seconds.");
					}
				}
				guiManager.refreshLayout();
				guiManager.AddSpacingSeparated();

				//--

				//// not required
				//guiManager.AddLabelBig("Debug", true, true);
				//guiManager.Add(bGui_Internal, OFX_IM_TOGGLE_ROUNDED_MINI);
				//guiManager.Add(bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);

				//guiManager.AddLabelBig("WORKFLOW"/*, true, true*/);
				guiManager.Add(bGuiWorkflow, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.AddTooltip("Automatically Hide or Show useful windows, \nor parts of the GUI, depending of the enabled sections \nor the usual App workflow.");
				guiManager.Add(guiManager.getGuiToggleOrganizer(), OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.AddTooltip("Panel for organizing and aligning the GUI windows \non the App window layout.");
				guiManager.Unindent();
			}
		}

		//--

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

	//if (manager.getAmountEffectsEnabled() == 0) return;

	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_MEDIUM;

	//// Constraint Window Shape
	//float w = 275;
	//ImVec2 size_min = ImVec2(w, -1);
	//ImVec2 size_max = ImVec2(w + 0, -1);
	//ImGui::SetNextWindowSizeConstraints(size_min, size_max);

	if (guiManager.beginWindowSpecial(bGui_Controls))
	{
		if (manager.getAmountEffectsEnabled() == 0)
		{
			string s;
			
			s = "You must Enable one or more FX Toggles!";
			guiManager.AddLabelBig(s, false, true);
			guiManager.AddSpacing();

			s = "Go to TOGGLES window and Enable some FX. \n";
			s += "You will see each FX's parameters here.";
			guiManager.AddLabel(s, false, true);

			guiManager.endWindowSpecial();

			return;
		}

		//--

		//TODO:
		// requires deep ofxSurfingImGui API remake
//#define SURFING_FIXING_COLLAPSE_GROUP
#ifdef SURFING_FIXING_COLLAPSE_GROUP
		static bool bExpanded_PRE = !bExpanded;

		if (guiManager.AddButton("Collapse", OFX_IM_BUTTON, 2)) {
			fg = ImGuiTreeNodeFlags_None;
			bExpanded = false;
		}
		guiManager.SameLine();
		if (guiManager.AddButton("Expand", OFX_IM_BUTTON, 2)) {
			fg = ImGuiTreeNodeFlags_DefaultOpen;
			bExpanded = true;
		}

		guiManager.AddSpacingSeparated();

		ImGuiCond cond = ImGuiCond_None;
		if (bExpanded_PRE != bExpanded) {
			bExpanded_PRE = bExpanded;
			cond = ImGuiCond_Once;
		}

		guiManager.AddGroup(manager.params_Controls, bExpanded, cond);
#else
		guiManager.AddGroup(manager.params_Controls);
#endif

		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGuiToggles()
{
	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

	if (guiManager.beginWindowSpecial(bGui_Toggles))
	{
		guiManager.Add(manager.bNone, OFX_IM_BUTTON, 2, true);
		guiManager.Add(manager.bAll, OFX_IM_BUTTON, 2);
		guiManager.AddSpacingSeparated();

		guiManager.AddGroup(manager.params_Toggles);

		guiManager.endWindowSpecial();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGui()
{
	guiManager.begin();
	{
		//guiManager.beginDocking();
		//{
		//}
		//guiManager.endDocking();

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

	//if (bEnable) manager.begin();
}

//---------------------------------------
void ofxSurfingFxPro::begin(ofCamera& cam) {//begin to draw scene inside to process!
	manager.begin(cam);

	//if (bEnable) manager.begin(cam);
	//else cam.begin();
}

//---------------------------------------
void ofxSurfingFxPro::end(bool autoDraw) {//ends the drawn scene and draws processed image!
	manager.end(autoDraw);

	//if (bEnable) manager.end(autoDraw);

	//manager.end();
	//if (bEnable) manager.end();
	//else cam.end();//why not required to end cam?
	//TODO: bug something happens after end... 2d stuff is drawn wrongly...
}

//--------------------------------------------------------------
void ofxSurfingFxPro::keyPressed(int key)
{
	if (bKeys_FX)
	{
		keyPressedFX(key);
		if (key == OF_KEY_LEFT_SHIFT) bShiftPressed = true;
	}

	if (key == OF_KEY_F1) bGui_Toggles = !bGui_Toggles;
	if (key == OF_KEY_F2) bGui_Controls = !bGui_Controls;
	if (key == OF_KEY_F3) presetsManager.bGui = !presetsManager.bGui;
	if (key == OF_KEY_F4) randomizer.bGui = !randomizer.bGui;

	//--

	//if (key == OF_KEY_F9) manager.doEnableNone();
	//if (key == OF_KEY_F10) doRandomFXAll(randomProb);

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

	if (bAutoSave) ofxSurfingHelpers::saveGroup(manager.params_Controls, path_GLOBAL + path_Params_Controls);

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

	// Presets only handles toggles!
//	// Undo Engine
//#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
//	presetsManager.undoManager.doSaveUndoWhenAuto();
//#endif
}
