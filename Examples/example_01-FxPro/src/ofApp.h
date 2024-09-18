
/*

	BUG:

	fix sometimes presets are overwritten


	TODO:

	add multi layout presets ui. 
		app modes workflow
		app mode: fx / presets. 
		improve workflow keys/edit/midi..

	Could add / split an extra randomizer for params only
	or disable the presets randomizer
	
	add bool param to doRandomize() to link to undo manual add

	fix undo path to one single folder

	ImGui
		split vec params in multi lines

	fix ImGui special windows
		link between add-ons
		main / master window. fix re arranging windows on start..
		link player window. rename. clean
		try to link presets window to parent fx pro

	fix ui path settings sometimes
	
	midi link
	log silencers
	add global disabler to pick some fx and disable the others

*/

//--
 
#define USE_WEBCAM // -> Optional

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

	ofxSurfingFxPro fxPro;

	ofxSurfingGui ui;

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
#ifdef USE_WEBCAM
	SurfingWebCam webcam;
	
	ofParameter<bool> bWebcamMode{ "Webcam Mode", false };
#endif

	bool bModeControl = false;

	//--

	ofxWindowApp w;
};
