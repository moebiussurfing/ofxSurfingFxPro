
/*

	TODO:

	fix undo path
	fix guiManager path settings sometimes

	add app help box
	randomizer addon to presets manager
		or surfing mood

	fix ImGui special windows
		link between addons
		main / master window. fix re arranging windows on start..
		link player window. rename. clean
		try to link presets window to parent fx pro
	
	midi link
	log silencers
	add global disable to pick hide some fx and hide the others
	app mode: fx / presets. improve workflow keys/edit/midi..

*/


#pragma once

#include "ofMain.h"

#include "ofxSurfingFxPro.h"
#include "ofxSurfingImGui.h"
#include "SurfingWebcam.h"

#define NUM_BOXES 100

class ofApp : public ofBaseApp
{

public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);
	void exit();

	ofParameterGroup params_ofApp{ "ofApp" };
	ofParameter<bool> bGui{ "ofApp", true };
	void drawGui();

	void setupScene();
	void drawScene();

	//--

	// Camera
	ofEasyCam cam;
	ofLight light;
	ofParameter<bool> bCamMouse{ "Mouse Camera", false };
	ofParameter<bool> bRotate{ "Rotate" , true };
	ofParameter<float> rotateSpeed{ "Speed", 0.1f , 0, 1 };
	ofParameter<bool> bLight{ "Light", true };
	ofParameterGroup params_Camera{ "Camera" };
	ofEventListener listener_bCamMouse;

	// Boxes
	vector<ofVec3f> posns;
	vector<ofColor> cols;
	ofVboMesh boxMesh;

	//--

	// Webcam
	SurfingWebcam webcam;
	
	ofParameter<bool> bWebcamMode{ "Webcam Mode", true };

	//--

	ofxSurfing_ImGui_Manager guiManager;

	ofxSurfingFxPro fxPro;
};
