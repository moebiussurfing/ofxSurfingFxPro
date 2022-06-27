#pragma once

#include "ofMain.h"

class SurfingWebcam
{
public:

	SurfingWebcam() { setup(); }
	~SurfingWebcam() { exitWebcam(); }

	void setup() { setupWebcamDevice(); }

private:
	
	ofVideoGrabber webcamGrab;
	ofParameter <std::string> webcamDeviceName{ "WEBCAM_DEVICE_NAME", "" };
	int _deviceIndex;
	ofTrueTypeFont font;
	
	void setupWebcamDevice();
	void exitWebcam();

	string path;

public:
	
	void update() { webcamGrab.update(); };
	void drawWebcam();
	
	void doNextWebcam();
	void doRestartWebcam();
	
	bool bDrawWebcamInfo = false;
	void drawInfo();
};

