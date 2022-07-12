#pragma once

#include "ofMain.h"

//----

#include "ofxSurfingVideoSkip.h"
#include "ofxSurfingImGui.h"
#include "ofxWindowApp.h"

class ofApp : public ofBaseApp 
{
public:
	
	void setup();
	void setupPlayer();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void exit();

	ofParameterGroup params_ofApp{ "ofApp" };
	ofParameter<bool> bGui{ "ofApp", true };

	ofxSurfing_ImGui_Manager guiManager;
	ofxSurfingVideoSkip player;
	ofxWindowApp w;

	void drawGui();
};