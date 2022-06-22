//
//  ofxDC_Utilities.h
//
//  Created by alp tugan on 26.08.2020.
//

#ifndef ofxDC_Utilities_h
#define ofxDC_Utilities_h

//#include "ofMain.h"

#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600

/* ALIGNMENT */
typedef enum {
    TOP_LEFT,
    TOP_RIGHT,
    TOP_MIDDLE,
    CENTER,
    CENTER_LEFT,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_MIDDLE,
    CUSTOM_POS
} ofxDC_ALIGNMENT;

class ofxDC_Utilities {
private:
    ofBitmapFont bf;
    float nextEventSeconds = 0;
public:
    // Split String from start
    string stringRemoveRange(string src, int begin, int end) {
        return src.erase(begin, end);
    }
    
    string stringReplace(string src, int begin, int end) {
        return src.substr(begin, end);
    }
    
    
    // Notify Per Second
    bool notifyPerSecond(float timePeriod) {
        bool result = false;
        float now = ofGetElapsedTimef();
        if(now > nextEventSeconds) {
            // do something here that should only happen
            // every 3 seconds
            nextEventSeconds = now + timePeriod;
            result = true;
        }
        
        return result;
    }
    
    // FLoat Modulo
    double findMod(double a, double b)
    {
        double mod;
        // Handling negative values
        if (a < 0)
            mod = -a;
        else
            mod =  a;
        if (b < 0)
            b = -b;

        // Finding mod by repeated subtraction

        while (mod >= b)
            mod = mod - b;

        // Sign of result typically depends
        // on sign of a.
        if (a < 0)
            return -mod;

        return mod;
    }

    // HEX TO FLOAT COLOR
    float hexToFloat(int v) {
        return float(v / 255.0f);
    }

    // Draw FPS
    void drawFPS(ofxDC_ALIGNMENT _position, int _x = 0, int _y = 0) {
        int x = 0;
        int y = 0;
        
        string fr = ofToString(ofGetFrameRate(),1);
        
        switch(_position) {
            case TOP_LEFT:
                x = 0;
                y = bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case TOP_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = bf.getBoundingBox(fr, 0, 0).getHeight();;
                break;
            case BOTTOM_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case BOTTOM_LEFT:
                x = 0;
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case TOP_MIDDLE:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case CENTER:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case CENTER_LEFT:
                x = 0;
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case CENTER_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case BOTTOM_MIDDLE:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case CUSTOM_POS:
                x = _x;
                y = _y;
                break;
        }
        
        ofDrawBitmapStringHighlight(fr, x, y);
    }


};

#endif /* ofxDC_Utilities_h */
