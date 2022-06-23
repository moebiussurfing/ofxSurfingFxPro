
/*

	TODO:

	add log slider for speed
	add guiWorkflow toggle
	link player window. rename. clean
	try to link presets window to parent fx pro
	add group presets for params
	add minimize and folders for helpers. settings.. collapse
	add randomizer addon
	add store settings bools no params. ofxPugiXML, ofxTOML
	player: copy drop down list to main window

*/


#pragma once

#include "ofMain.h"

#include "ofxSurfingFxPro.h"
#include "ofxSurfingImGui.h"

#define NUM_BOXES 100

class ofApp : public ofBaseApp 
{

public:

	void setup();
	void update();
	void draw();
	void drawGui();
	void drawScene();

	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxSurfing_ImGui_Manager guiManager;

	ofxSurfingFxPro fx;

	// Camera
	ofEasyCam cam;
	bool bCam = false;
	bool bRot = true;
	float speed = 0.1f;
	bool bLight = true;
	ofLight light;

	// Boxes
	vector<ofVec3f> posns;
	vector<ofColor> cols;
	ofVboMesh boxMesh;
};
