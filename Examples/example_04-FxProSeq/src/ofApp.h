
/*

	BUG:

	fix sometimes presets are overwritten


	TODO:

	add multi layout presets ui. 
		app modes workflow
		app mode: fx / presets. 
		improve workflow keys/edit/midi..

	add bool param to doRandomize() to link to undo manual add

	fix ui path settings sometimes
	
*/

//--

//#define USE_WEBCAM // -> Optional

//----

#pragma once

#include "ofMain.h"

#include "ofxSurfingFxPro.h"
#include "ofxSurfingImGui.h"

#ifdef USE_WEBCAM
	#include "SurfingWebcam.h"
#endif

#include "ofxWindowApp.h"

#define NUM_BOXES 100

//--

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);
	void exit();

	ofxSurfingFxPro fxPro;

	ofxSurfingGui ui;

	ofParameterGroup params_ofApp { "ofApp" };
	ofParameter<bool> bGui { "ofApp", true };
	void drawGui();

	void setupScene();
	void drawScene();

	//--

	// Camera
	ofEasyCam cam;
	ofLight light;
	ofParameter<bool> bCamMouse { "Mouse Camera", false };
	ofParameter<bool> bRotate { "Rotate", true };
	ofParameter<float> rotateSpeed { "Speed", 0.1f, 0, 1 };
	ofParameter<bool> bLight { "Light", true };
	ofParameterGroup params_Camera { "Camera" };
	ofEventListener listener_bCamMouse;

	//--

	// Scene Boxes
	vector<ofVec3f> posns;
	vector<ofColor> cols;
	ofVboMesh boxMesh;

	//--

	// Webcam
#ifdef USE_WEBCAM
	SurfingWebCam webcam;

	ofParameter<bool> bWebcamMode { "Webcam Mode", false };
#endif

	bool bModeControl = false;

	//--

	ofxWindowApp w;
};
