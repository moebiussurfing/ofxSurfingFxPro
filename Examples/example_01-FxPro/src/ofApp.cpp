#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//w.setFrameRate(50);
	//w.setVerticalSync(false);
	w.setup();

	ofSetBackgroundColor(20);

	setupScene();

	//--

	// App Session Settings

#ifdef USE_WEBCAM
	webcam.bKeys.makeReferenceTo(ui.bKeys);
	params_Camera.add(bWebcamMode);
#endif
	params_Camera.add(bCamMouse);
	params_Camera.add(bRotate);
	params_Camera.add(rotateSpeed);
	params_Camera.add(bLight);

	params_ofApp.add(params_Camera);
	params_ofApp.add(bGui);

	ofxSurfingHelpers::load(params_ofApp);

	//--

	// Gui
	ui.setName("ofApp");
	ui.setup();
}

//--------------------------------------------------------------
void ofApp::setupScene() {

	// Boxes
	// create our own box mesh as there is a bug with
	// normal scaling and ofDrawBox() at the moment
	const int szScene = 500;
	const int szBox = szScene / 11.f;

	boxMesh = ofMesh::box(szBox, szBox, szBox);

	// Setup box positions
	for (unsigned i = 0; i < NUM_BOXES; ++i)
	{
		posns.push_back(ofVec3f(
			ofRandom(-szScene, szScene),
			ofRandom(-szScene, szScene),
			ofRandom(-szScene, szScene)));
		cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
	}

	// Cam
	cam.disableMouseInput();
	cam.setupPerspective();
	cam.setPosition(0, -szScene / 2, szScene);
	cam.lookAt(glm::vec3(0, 2 * szBox, 0));

	listener_bCamMouse = bCamMouse.newListener([this](bool&)
		{
			ofLogNotice("ofApp") << "bCamMouse: " << bCamMouse;
			if (bCamMouse) cam.enableMouseInput();
			else cam.disableMouseInput();
		});

	// Light
	light.setPosition(0, -2 * szScene, szScene);
}

//--------------------------------------------------------------
void ofApp::update()
{
	//--

	// FxPro
#ifdef USE_WEBCAM
	if (bWebcamMode)
	{
		fxPro.begin();
		{
			webcam.drawWebCam();
		}
		fxPro.end(false);
	}
	else
#endif
	{
		if (bLight) {
			ofEnableLighting();
			light.enable();
		}

		fxPro.begin(cam);
		{
			drawScene();
		}
		fxPro.end(false);

		if (bLight) {
			light.disable();
			ofDisableLighting();
		}
	}

	//--

	static bool bModeControl_PRE = !bModeControl;
	if (bModeControl != bModeControl_PRE) 
	{
		bModeControl_PRE = bModeControl;
		bCamMouse = bModeControl;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofEnableDepthTest();

	// FxPro
	fxPro.draw();

	//----

	if (!bGui) return;

	ofDisableDepthTest();

	drawGui();

#ifdef USE_WEBCAM
	if (bWebcamMode) webcam.drawInfo();
#endif
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	// FxPro
	fxPro.drawGui();

	//--

	// ofApp ImGui Window
	ui.Begin();
	{
		// Set Window Shape
		{
			float pad = 5;
			ImVec2 sz = ImVec2(250, 400); // final size could vary when auto resize is enabled...
			ImVec2 pos = ImVec2(ofGetWidth() - sz.x - pad, pad);
			ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(sz, ImGuiCond_FirstUseEver);
		}

		// Constraint size
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (ui.BeginWindow("ofApp"))
		{
			ui.Add(fxPro.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);

			//--

			// Webcam
#ifdef USE_WEBCAM
			{
				ui.AddSpacingBigSeparated();
				ui.AddLabelBig("Webcam", true, true);
				ui.Add(bWebcamMode);
				if (bWebcamMode)
				{
					ui.Add(ui.bKeys, OFX_IM_TOGGLE_ROUNDED);
					if (ui.AddButton("Next", OFX_IM_BUTTON, 2, true))
					{
						webcam.doNextWebCam();
					}
					if (ui.AddButton("Restart", OFX_IM_BUTTON, 2))
					{
						webcam.doRestartWebCam();
					}
					ImGui::Checkbox("Draw info", &webcam.bDrawWebCamInfo);
				}
			}

			// Scene Boxes
			if (!bWebcamMode)
#endif
			{
				ui.AddSpacingBigSeparated();
				ui.AddLabelBig("Camera", true, true);
				ui.Add(bCamMouse);
				ui.Add(bRotate);
				if (bRotate) ui.Add(rotateSpeed);
				ui.Add(bLight);
			}

			ui.EndWindow();
		}
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofEnableDepthTest();

	ofPushMatrix();

	// Rotate
	{
		static float r;
		float step = ofMap(rotateSpeed, 0, 1, 0.025f, 5);
		if (bRotate) r += step;
		r = ofWrapDegrees(r);
		ofRotateYDeg(r);
	}

	// Draw Boxes
	for (unsigned i = 0; i < posns.size(); ++i)
	{
		ofSetColor(cols[i]);
		ofPushMatrix();
		ofTranslate(posns[i]);
		boxMesh.draw();
		ofPopMatrix();
	}

	if (fxPro.bDebug) ofDrawAxis(100);

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Hide all GUI
	if (key == 'G') bGui = !bGui;

	if (key == 'M') bModeControl = true;

	//--

	fxPro.keyPressed(key);

	//--

	// Webcam
#ifdef USE_WEBCAM
	if (bWebcamMode)
	{
		if (webcam.bKeys)
		{
			// Open next device
			if (key == 'D') webcam.doNextWebCam();

			// Restart devices
			if (key == 'R') webcam.doRestartWebCam();
		}
	}
#endif
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'M') bModeControl = false;

	fxPro.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	fxPro.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofxSurfingHelpers::save(params_ofApp);

	w.save();
}
