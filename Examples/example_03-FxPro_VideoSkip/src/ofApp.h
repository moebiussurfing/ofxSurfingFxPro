#pragma once

#include "ofMain.h"

//----
/*

TODO:

	+ auto midi knobs to FX:
		set the master 6 knobs.
		auto assign to these 6 knobs 
		from current GUI editing / first plane page.

*/

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

	void drawGui();

	ofxSurfing_ImGui_Manager guiManager;

	ofParameterGroup params_ofApp{ "ofApp" };
	ofParameter<bool> bGui{ "ofApp", true };

	ofxSurfingVideoSkip player;

	ofxWindowApp w;
};