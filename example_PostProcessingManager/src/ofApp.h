#pragma once

#include "ofMain.h"
#include "ofxDC_Utilities.h"
#include "ofxPostProcessingManager.h"

#define NUM_BOXES 100

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxPostProcessingManager manager;
    ofxDC_Utilities notifier;
    ofEasyCam cam;
    
    ofLight light;
    
    // boxes
    vector<ofVec3f> posns;
    vector<ofColor> cols;
    ofVboMesh boxMesh;
    
    bool hideGui;
    bool automateEffects;
};
