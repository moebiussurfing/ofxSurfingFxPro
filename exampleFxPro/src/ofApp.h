
/*

	TODO:
	
	fix organizer window width
	fix recall app setings / presets positions

	add app mode: fx / presets
	add undo adon to presets manager
	add randomizer addon to presets manager
	log slider for speed, prob, rotate
	key latch toggle
	fix ImGui special windows
		link between addons
		main / master window
	
	link player window. rename. clean
	try to link presets window to parent fx pro
	group presets for params
	minimize and folders for helpers. settings.. collapse

	store settings bools, rotate, speed etc without params. ofxPugiXML, ofxTOML

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

	void setupScene();
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
