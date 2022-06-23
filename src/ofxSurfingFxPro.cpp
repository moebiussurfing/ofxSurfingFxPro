#include "ofxSurfingFxPro.h"

//--------------------------------------------------------------
ofxSurfingFxPro::ofxSurfingFxPro()
{
	path_GLOBAL = "ofxSurfingFxPro/"; // this is to folder all files to avoid mixing with other add-ons data
	path_Params_AppSettings = "FxPro_Settings.xml";

	setup();
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
void ofxSurfingFxPro::setup()
{
	ofxSurfingHelpers::setThemeDark_ofxGui();

	// Setup manager
	manager.setup(ofGetWidth(), ofGetHeight());

	// Load Settings
	manager.loadSettings();

	//--

	// Gui
	guiManager.setWindowsMode(IM_GUI_MODE_WINDOWS_SPECIAL_ORGANIZER);
	guiManager.setup();

	guiManager.addWindowSpecial(bGui);
	guiManager.addWindowSpecial(bGui_Toggles);
	guiManager.addWindowSpecial(bGui_Controls);

	guiManager.startup();

	refreshStyles();

	//--
	 
	// Callbacks
	params.setName("Callbacks");
	params.add(bRandom);
	ofAddListener(params.parameterChangedE(), this, &ofxSurfingFxPro::Changed);

	// Session Settings
	params_AppSettings.setName("FxPro");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_Controls);
	params_AppSettings.add(bGui_Toggles);
	params_AppSettings.add(bGui_Internal);
	params_AppSettings.add(bAutomate);
	params_AppSettings.add(bDebug);

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);

	// Get notified when toggles changed!
	ofAddListener(manager.params_Toggles.parameterChangedE(), this, &ofxSurfingFxPro::Changed_Enablers);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::refreshStyles()
{
	guiManager.clearStyles();

	// Customize all toggles inside the group 
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

	guiManager.AddStyleGroup(manager.gFxaaGroup,		(manager.bEnablers[0] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gBloomGroup,		(manager.bEnablers[1] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gKaliGroup,		(manager.bEnablers[2] ? tON : tOFF), fg);

	guiManager.AddStyleGroup(manager.gGodRaysGroup,		(manager.bEnablers[5] ? tON : tOFF), fg);

	guiManager.AddStyleGroup(manager.gSsaoGroup,		(manager.bEnablers[7] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gZoomBlurGroup,	(manager.bEnablers[8] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gRGBGroup,			(manager.bEnablers[9] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gFilmGrainGroup,	(manager.bEnablers[10] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gDotScreenGroup,	(manager.bEnablers[11] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gGlicthGroup,		(manager.bEnablers[12] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gBadTVGroup,		(manager.bEnablers[13] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gcolorACESGroup,	(manager.bEnablers[14] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gNoiseGroup,		(manager.bEnablers[15] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gTiltShiftGroup,	(manager.bEnablers[16] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gSupGroup,			(manager.bEnablers[17] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gGliAutoGroup,		(manager.bEnablers[18] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gSpaceColorGroup,	(manager.bEnablers[19] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gDitherGroup,		(manager.bEnablers[20] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gStrobberGroup,	(manager.bEnablers[21] ? tON : tOFF), fg);
	guiManager.AddStyleGroup(manager.gRimbLightGroup,	(manager.bEnablers[22] ? tON : tOFF), fg);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::update()
{
	if (bAutomate)
		if (notifier.notifyPerSecond(0.5))
		{
			manager.disableAll();
			manager.switchFX(int(ofRandom(manager.getEffectNum())));
		}

	manager.updateValues();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawGui() {

	if (!bGui) return;

	drawImGui();

	if (bGui_Internal)
	{
		manager.drawGui();
	}

	if (bDebug)
	{
		manager.drawDebug();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawImGui()
{
	guiManager.begin();
	{
		//--

		// Main
		
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (guiManager.beginWindowSpecial(bGui))
		{
			guiManager.AddLabelHuge("Fx Pro");
			guiManager.Add(bGui_Toggles, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			guiManager.Add(bGui_Controls, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			guiManager.AddSpacing();
			guiManager.Add(guiManager.getGuiToggleOrganizer(), OFX_IM_TOGGLE_ROUNDED_SMALL);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Helpers", true, true);
			guiManager.Add(bRandom, OFX_IM_BUTTON);
			guiManager.Add(manager.gdisableAll, OFX_IM_BUTTON);
			guiManager.Add(bAutomate, OFX_IM_TOGGLE_BORDER_BLINK);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Settings", true, true);
			guiManager.Add(manager.btnLoad);
			guiManager.Add(manager.btnSave);
			guiManager.AddSpacingBigSeparated();

			guiManager.AddLabelBig("Debug", true, true);
			guiManager.Add(bGui_Internal, OFX_IM_TOGGLE_ROUNDED_MINI);
			guiManager.Add(bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);

			guiManager.endWindowSpecial();
		}

		//--

		// Toggles 
		
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

		if (guiManager.beginWindowSpecial(bGui_Toggles))
		{
			guiManager.AddGroup(manager.params_Toggles);

			guiManager.endWindowSpecial();
		}

		//--
		
		// Controls

		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_BIG;

		if (guiManager.beginWindowSpecial(bGui_Controls))
		{
			guiManager.AddGroup(manager.params_Controls);

			guiManager.endWindowSpecial();
		}
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
	if (key == 'd') manager.disableAll();

	else if (key == '1') manager.loadSettings("1.json");
	else if (key == '2') manager.loadSettings("2.json");
	else if (key == '3') manager.loadSettings("3.json");

	else if (key == 'g') bGui = !bGui;
	else if (key == 'r') doRandom();
	else if (key == 'a') bAutomate = !bAutomate;
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
void ofxSurfingFxPro::doRandom() {
	manager.disableAll();
	int randId = int(ofRandom(manager.getEffectNum()));
	manager.switchFX(randId);
}

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (name == bRandom.getName() && bRandom)
	{
		bRandom = false;

		doRandom();
	}
}

//--------------------------------------------------------------
void ofxSurfingFxPro::Changed_Enablers(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	refreshStyles();
}