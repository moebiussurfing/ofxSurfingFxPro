#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);
	ofSetWindowPosition(-1920, 25);

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

	// Light
	light.setPosition(0, -1000, 500);

	guiManager.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString((int)ofGetFrameRate()));

	fx.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofEnableDepthTest();

	if (bLight) light.enable();

	// Begin scene to post process
	fx.begin(cam);
	{
		drawScene();
	}
	// End scene and draw
	fx.end();

	if (bLight) light.disable();
	ofDisableLighting();

	//----

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	ofDisableDepthTest();

	fx.drawGui();

	//--

	guiManager.begin();
	{
		// Window shape
		{
			float pad = 5;
			ImVec2 sz = ImVec2(250, 400);
			ImVec2 pos = ImVec2(ofGetWidth() - sz.x - pad, pad);
			ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(sz, ImGuiCond_FirstUseEver);
		}

		if (guiManager.beginWindow("ofApp"))
		{
			guiManager.Add(fx.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);

			guiManager.AddSeparator();

			guiManager.AddLabelBig("Camera");
			if (ImGui::Checkbox("Mouse", &bCam)) {
				if (bCam) cam.enableMouseInput();
				else cam.disableMouseInput();
			}
			ImGui::Checkbox("Light", &bLight);
			ImGui::Checkbox("Rot", &bRot);
			if (bRot) ImGui::SliderFloat("Speed", &speed, 0, 1);

			guiManager.endWindow();
		}
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofPushMatrix();

	if (bRot) {
		float s = ofMap(speed, 0, 1, 6000, 30);
		float r = ofMap(ofGetFrameNum() % (int)s, 0, s, 0, 360, true);
		ofRotateYDeg(r);
	}

	// Draw boxes
	for (unsigned i = 0; i < posns.size(); ++i)
	{
		ofSetColor(cols[i]);
		ofPushMatrix();
		ofTranslate(posns[i]);
		boxMesh.draw();
		ofPopMatrix();
	}

	if (fx.bDebug) ofDrawAxis(100);

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	fx.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	fx.windowResized(w, h);
}