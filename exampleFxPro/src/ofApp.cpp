#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);

	// Setup box positions
	for (unsigned i = 0; i < NUM_BOXES; ++i)
	{
		posns.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
		cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
	}

	// Setup light
	light.setPosition(1000, 1000, 2000);

	// create our own box mesh as there is a bug with
	// normal scaling and ofDrawBox() at the moment
	boxMesh = ofMesh::box(20, 20, 20);

}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	fx.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// setup gl state
	ofEnableDepthTest();
	light.enable();

	// begin scene to post process
	fx.begin(cam);
	{
		// draw boxes
		for (unsigned i = 0; i < posns.size(); ++i)
		{
			ofSetColor(cols[i]);
			ofPushMatrix();
			ofTranslate(posns[i]);
			boxMesh.draw();
			ofPopMatrix();
		}

		ofDrawAxis(100);

	}
	// end scene and draw
	fx.end();

	//light.disable();

	//----

	fx.drawGui();
	fx.drawImGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	fx.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	fx.windowResized(w, h);
}