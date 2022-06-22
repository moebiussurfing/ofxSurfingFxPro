#pragma once

#include "ofMain.h"
#include "ofxSurfingFxPro.h"

#define NUM_BOXES 100

class ofApp : public ofBaseApp 
{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxSurfingFxPro fx;

	ofEasyCam cam;

	ofLight light;

	// boxes
	vector<ofVec3f> posns;
	vector<ofColor> cols;
	ofVboMesh boxMesh;

};
