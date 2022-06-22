#include "ofxSurfingFxPro.h"

//--------------------------------------------------------------
ofxSurfingFxPro::ofxSurfingFxPro() {
	setup();
}

//--------------------------------------------------------------
ofxSurfingFxPro::~ofxSurfingFxPro() {
	exit();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::exit() {
}

//--------------------------------------------------------------
void ofxSurfingFxPro::setup() {

	ofxSurfingHelpers::setThemeDarkMini_ofxGui();

	// setup manager
	manager.setup(ofGetWidth(), ofGetHeight());

	// Load Settings
	manager.loadSettings();
}

//--------------------------------------------------------------
void ofxSurfingFxPro::update() {

	if (notifier.notifyPerSecond(0.5) && automateEffects) {
		manager.disableAll();
		manager.switchFX(int(ofRandom(manager.getEffectNum())));
	}

	manager.updateValues();
}

////--------------------------------------------------------------
//void ofxSurfingFxPro::draw() {
//	ofEnableDepthTest();
//
//	//manager.dra
//}

//--------------------------------------------------------------
void ofxSurfingFxPro::drawGui() {

	ofDisableDepthTest();

	if (!hideGui) manager.drawGui();

	//if (!hideGui) {
	//	manager.drawGui();
	//	//manager.drawGui(ofGetWidth() - 220, 0);
	//}
	//else {
	//	// There is a kind of buggy issue on MAC.
	//	// Drawing the GUI out of the window.
	//	// Otherwise FPS draws continously
	//	manager.hideGui();
	//}
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
void ofxSurfingFxPro::keyPressed(int key) {
	if (key == 'd')
		manager.disableAll();

	else if (key == '1')
		manager.loadSettings("scene1.json");

	else if (key == '2')
		manager.loadSettings("scene2.json");

	else if (key == '3')
		manager.loadSettings("scene3.json");

	else if (key == 'h')
		hideGui = !hideGui;

	else if (key == 'r') {
		manager.disableAll();
		int randId = int(ofRandom(manager.getEffectNum()));
		manager.switchFX(randId);
	}

	else if (key == 'a')
		automateEffects = !automateEffects;
}

//--------------------------------------------------------------
void ofxSurfingFxPro::windowResized(int w, int h) {
	manager.windowResized(w, h);
}