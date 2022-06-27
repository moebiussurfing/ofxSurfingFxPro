#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);
	ofSetWindowPosition(-1920, 25);

	setupScene();

	// Webcam
	setupWebcamDevice();

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

	//--

	// Settings
	params_Camera.add(bWebcamMode);
	params_Camera.add(bCamMouse);
	params_Camera.add(bRotate);
	params_Camera.add(rotateSpeed);
	params_Camera.add(bLight);

	ofxSurfingHelpers::load(params_Camera);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString((int)ofGetFrameRate()));

	webcamGrab.update();

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
			drawWebcam();
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

	if (bWebcamMode && bDrawWebcamInfo) drawWebcamInfo();
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
					doNextWebcam();
				}if (guiManager.AddButton("Restart", OFX_IM_BUTTON, 2)) {
					doRestartWebcam();
				}
				ImGui::Checkbox("Draw info", &bDrawWebcamInfo);
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
	//if (key == OF_KEY_F1) bGui = !bGui;

	//--

	fxPro.keyPressed(key);

	//--

	// Webcam
	if (bWebcamMode)
	{
		// Select next device
		if (key == 'D') doNextWebcam();

		// Restart device
		if (key == 'R') doRestartWebcam();
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
void ofApp::exit() {

	exitWebcam();
	ofxSurfingHelpers::save(params_Camera);
}

//--

// Webcam

//--------------------------------------------------------------
void ofApp::setupWebcamDevice()
{
	string str = "overpass-mono-bold.otf";
	string pathFont = "assets/fonts/" + str; // data/assets/
	bool b = font.load(pathFont, 10);
	if (!b) font.load(OF_TTF_MONO, 10);

	// load settings
	ofXml _xml;
	bool _isLoaded;
	_isLoaded = _xml.load("settings.xml");
	ofDeserialize(_xml, webcamDeviceName);
	ofLogNotice(__FUNCTION__) << _xml.toString();

	// start
	auto _devices = webcamGrab.listDevices();
	_deviceIndex = -1;
	if (_isLoaded) {
		for (int i = 0; i < _devices.size(); i++) {
			if (_devices[i].deviceName == webcamDeviceName.get()) {
				_deviceIndex = i;
				ofLogNotice(__FUNCTION__) << "device name:\t" << webcamDeviceName.get();
				ofLogNotice(__FUNCTION__) << "device index:\t" << _deviceIndex;
			}
		}
	}
	if (_deviceIndex == -1) {//error. try to load first device...
		_deviceIndex = 0;//select cam device
		webcamDeviceName = _devices[_deviceIndex].deviceName;
	}

	// Start device
	webcamGrab.setVerbose(true);
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}
//--------------------------------------------------------------
void ofApp::drawWebcamInfo()
{
	// Display device name
	string str;
	str += "WEBCAM #" + ofToString(_deviceIndex) + " \n";
	str += webcamDeviceName.get();
	//str += " " + ofToString(webcamGrab.isInitialized() ? "ON" : "OFF");
	str += "\n";
	str += "D NEXT \n";
	str += "R RESTART \n";

	int xx = 30;
	int yy = 40;

	if (!font.isLoaded()) ofDrawBitmapStringHighlight(str, xx, yy);
	else
	{
		ofPushStyle();

		// bbox
		ofSetColor(0, 150);
		ofFill();
		ofRectangle _r(font.getStringBoundingBox(str, xx, yy));
		int pad = 20;
		_r.setWidth(_r.getWidth() + pad);
		_r.setHeight(_r.getHeight() + pad);
		_r.setX(_r.getPosition().x - pad / 2.);
		_r.setY(_r.getPosition().y - pad / 2.);
		//_r.scaleFromCenter(1.05, 1.5);
		ofDrawRectRounded(_r, 5.);

		// text
		ofSetColor(255);
		ofNoFill();
		font.drawString(str, xx, yy);

		ofPopStyle();
	}
}
//--------------------------------------------------------------
void ofApp::drawWebcam() {
	ofRectangle r(0, 0, webcamGrab.getWidth(), webcamGrab.getHeight());
	r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_CENTER);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_STRETCH_TO_FILL);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_FILL);
	webcamGrab.draw(r.x, r.y, r.width, r.height);
}

//--------------------------------------------------------------
void ofApp::exitWebcam() {
	ofXml _xml;
	ofSerialize(_xml, webcamDeviceName);
	_xml.save("settings.xml");
}

//--------------------------------------------------------------
void ofApp::doRestartWebcam() {

	webcamGrab.close();
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}
//--------------------------------------------------------------
void ofApp::doNextWebcam() {
	auto _devs = webcamGrab.listDevices();
	_deviceIndex++;
	if (_deviceIndex > _devs.size() - 1) _deviceIndex = 0;
	webcamDeviceName = _devs[_deviceIndex].deviceName;

	webcamGrab.close();
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}