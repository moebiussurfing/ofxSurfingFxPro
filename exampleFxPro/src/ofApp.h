
/*

	TODO:

	add app help box
	randomizer addon to presets manager
		or surfing mood
	midi link
	log silencers
	add global disable to pick hide some fx and hide the others
	app mode: fx / presets

	fix ImGui special windows
		link between addons
		main / master window. fix re arranging windows on start..
		link player window. rename. clean
		try to link presets window to parent fx pro

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
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);
	void exit();

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
	ofVideoGrabber webcamGrab;
	ofParameter<bool> bWebcamMode{ "Webcam Mode", true };
	ofParameter <std::string> webcamDeviceName{ "WEBCAM_DEVICE_NAME", "" };
	int _deviceIndex;
	ofTrueTypeFont font;
	void setupWebcamDevice();
	void drawWebcam();
	void drawWebcamInfo();
	void exitWebcam();
	void doNextWebcam();
	void doRestartWebcam();
	bool bDrawWebcamInfo = false;

	//--

	ofxSurfing_ImGui_Manager guiManager;

	ofxSurfingFxPro fxPro;
};
