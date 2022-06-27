#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);
	ofSetWindowPosition(-1920, 25);

	setupScene();

	//--

	// Settings
	params_Camera.add(bWebcamMode);
	params_Camera.add(bCamMouse);
	params_Camera.add(bRotate);
	params_Camera.add(rotateSpeed);
	params_Camera.add(bLight);
	params_ofApp.add(params_Camera);

	params_ofApp.add(bGui);

	ofxSurfingHelpers::load(params_ofApp);

	//--

	// Gui
	guiManager.setName("ofApp");
	guiManager.setup();
}

//--------------------------------------------------------------
void ofApp::setupScene() {

	// Boxes
	// create our own box mesh as there is a bug with
	// normal scaling and ofDrawBox() at the moment
	boxMesh = ofMesh::box(20, 20, 20);

	// Setup box positions
	for (unsigned i = 0; i < NUM_BOXES; ++i)
	{
		posns.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
		cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
	}

	// Cam
	cam.disableMouseInput();
	cam.setupPerspective();
	cam.setPosition(0, -250, 500);
	cam.lookAt(glm::vec3(0));

	listener_bCamMouse = bCamMouse.newListener([this](bool&)
		{
			ofLogNotice("ofApp") << "bCamMouse: " << bCamMouse;
			if (bCamMouse) cam.enableMouseInput();
			else cam.disableMouseInput();
		});

	// Light
	light.setPosition(0, -1000, 500);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString((int)ofGetFrameRate()));

	webcam.update();

	// FxPro
	fxPro.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofEnableDepthTest();

	if (bWebcamMode)
	{
		fxPro.begin();
		{
			webcam.drawWebcam();
		}
		fxPro.end();
	}
	else
	{
		if (bLight) light.enable();

		fxPro.begin(cam);
		{
			drawScene();
		}
		fxPro.end();

		if (bLight) light.disable();
		ofDisableLighting();
	}

	//----

	if (!bGui) return;

	ofDisableDepthTest();

	drawGui();

	if (bWebcamMode) webcam.drawInfo();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	// FxPro
	fxPro.drawGui();

	//--

	guiManager.begin();
	{
		// Set Window Shape
		float pad = 5;
		ImVec2 sz = ImVec2(250, 400); // final size could vary when auto resize is enabled...
		ImVec2 pos = ImVec2(ofGetWidth() - sz.x - pad, pad);
		ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(sz, ImGuiCond_FirstUseEver);

		// Constraint size
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

		if (guiManager.beginWindow("ofApp"))
		{
			guiManager.Add(fxPro.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			guiManager.AddSpacingBigSeparated();

			// Webcam
			guiManager.AddLabelBig("Webcam", true, true);
			guiManager.Add(bWebcamMode);
			if (bWebcamMode)
			{
				if (guiManager.AddButton("Next", OFX_IM_BUTTON, 2, true)) {
					webcam.doNextWebcam();
				}if (guiManager.AddButton("Restart", OFX_IM_BUTTON, 2)) {
					webcam.doRestartWebcam();
				}
				ImGui::Checkbox("Draw info", &webcam.bDrawWebcamInfo);
			}

			// Scene Boxes
			if (!bWebcamMode)
			{
				guiManager.AddSpacingBigSeparated();
				guiManager.AddLabelBig("Camera", true, true);
				guiManager.Add(bCamMouse);
				guiManager.Add(bRotate);
				if (bRotate) guiManager.Add(rotateSpeed);
				guiManager.Add(bLight);
			}

			guiManager.endWindow();
		}
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofPushMatrix();

	if (bRotate)
	{
		static float r;
		float step = ofMap(rotateSpeed, 0, 1, 0.025f, 5);
		r += step;
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
	// Hide all gui
	if (key == 'G') bGui = !bGui;

	//--

	fxPro.keyPressed(key);

	//--

	// Webcam
	if (bWebcamMode)
	{
		// Select next device
		if (key == 'D') webcam.doNextWebcam();

		// Restart device
		if (key == 'R') webcam.doRestartWebcam();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	fxPro.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	fxPro.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::exit() 
{
	ofxSurfingHelpers::save(params_ofApp);
}
