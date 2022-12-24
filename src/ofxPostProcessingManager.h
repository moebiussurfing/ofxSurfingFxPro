#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxPostProcessing.h"
#include "ofxDC_Utilities.h"

class ofxPostProcessingManager {

public:

	ofxPostProcessingManager();
	~ofxPostProcessingManager();

	void setup(int w, int h);

	void drawGui();
	void drawDebug();

	void windowResized(int w, int h);

	void begin();
	void begin(ofCamera& cam);
	void end(bool autoDraw = true);

	//--------------------------------------------------------------
	void draw(bool bEnabled = true)
	{
		if (bEnabled) {
			//draw processed
			ofPushStyle();
			ofSetColor(255, 255, 255);
			post.draw();
			ofPopStyle();
		}
		else {
			//draw un-processed
			//requires to be flipped..
			float w = ofGetWidth();
			float h = ofGetHeight();
			post.getRawRef().draw(0, h, w, -h);
		}
	};


	void updateFX();

private:

	void setupGui();
	void exit();

public:

	void saveSettings(string fileName);
	void loadSettings(string fileName = "");

	// disable all effects
	void doEnableNone();
	void doEnableAll();
	void doResetAll();

	void doRefreshFX(); // applies toggle states. required when params are modified without triggering...
	// pr.setWithoutEventNotifications(v);

	// switch state for an effect
	void doToggleFX(int postId);
	void doPowerFX(int postId, bool bState);

	// getter & setters
	int getAmountEffects();

private:

	ofxDC_Utilities utils;

	FxaaPass::Ptr fxaa;
	BloomPass::Ptr bloomPass;
	KaleidoscopePass::Ptr kaleidoscope;
	NoiseWarpPass::Ptr noiseWrap;
	PixelatePass::Ptr pixelate;
	GodRaysPass::Ptr godRays;
	LimbDarkeningPass::Ptr limbDarkening;
	SSAOPass::Ptr ssao;
	ZoomBlurPass::Ptr zoomBlur;

	RGBShiftPass::Ptr rgbPass;

	FilmGrainLinesPass::Ptr filmGrainLinesPass;
	DotScreenPass::Ptr dotScreenPass;
	DigitalGlitchPass::Ptr glitchPass;

	BadTVPass::Ptr badTv;
	ACESFilmicToneMappingPass::Ptr colorACES;
	NoisePass::Ptr noiseGrain;
	TiltShiftPass::Ptr tiltShift;
	SuperPass::Ptr superShader;
	GlitchAutomatedPass::Ptr glitchAuto;
	SpaceColorMovePass::Ptr spaceColor;
	DitherPass::Ptr dither;
	ColorInvert::Ptr strobber;

	RimHighlightingPass::Ptr rimbShift;

	// Becareful when enabling a pass...
	//ContrastPass::Ptr contrastShift;
	//FogPass::Ptr fogFilter;
	//DofPass::Ptr dof;
	//FirePass::Ptr lavaFire;
	//SlantShiftPass::Ptr slantShift;

private:

	void initializeFX();

	ofxPostProcessing post;

	//------------------------------------------

	ofxPanel gui;

public:

	ofxToggle gDebugDraw;

private:

	// Settings

	// not used. could be removed
	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void savePresetPressed();
	void loadPreset();

public:

	// not used. could be removed
	ofParameter<void> btnLoad = { "Load" };
	ofParameter<void> btnSave = { "Save" };

	ofParameter<void> bNone = { "NONE" };
	ofParameter<void> bAll = { "ALL" };
	ofParameter<void> bReset = { "RESET" };
	ofParameter<bool> bSolo{ "SOLO", false };

private:

	string fileName;

	void Changed_bNone();
	void Changed_bAll();
	void Changed_bReset();

	ofEventListeners listeners_bReset;

	// 0 Fxaa
	void doReset_FXAA() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset FXAA");
		gFxaaDivMin = 128.0;
		gFxaaDivMul = 8.0;
		gFxaaSpanMax = 8.0;
	};
	// 1 Bloom
	void doReset_Bloom() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset Bloom");
		gBloomBlurX = 0.000953125;
		gBloomBlurY = 0.000953125;
	};
	// 2 Kaleidoscope
	void doReset_Kaleidoscope() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset Kaleidoscope");
		gKaleiSegments = 2.f;
	};
	// 5 GodRays
	void doReset_GodRays() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset GodRays");
		gGodRaysLightDotView = 0.3;
		gLightPositionOnScreen = glm::vec3(0);
	};
	// 7 Ssao
	void doReset_Ssao() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset Ssao");
		gSsaofogEnabled = false;
		gSsaofogNear = 1;
		gSsaofogFar = 1000;
		gSsaocameraNear = 1;
		gSsaocameraFar = 1000;
		gSsaoonlyAO = false;
		gSsaoAoClamp = 0.65;
	};
	// 8 ZoomBlur
	void doReset_ZoomBlur() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset ZoomBlur");
		gZoomCenterXY = glm::vec2(0.5, 0.5);
		gZoomWeight = 0.25;
		gZoomDensity = 0.25;
		gZoomExposure = 0.48;
	};
	// 9 RGB 
	void doReset_RGB() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset RGB");
		gRGBAngle = 0;
		gRGBAmount = 0.005;
	};
	// 10 FilmGrainLines  
	void doReset_FilmGrainLines() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset FilmGrainLines");
		gFilmGrainLCount = 1096;
		gFilmGrainLGrayScale = false;
		gFilmGrainLnIntensity = 0.5;
		gFilmGrainLsIntensity = 0.5;
	};
	// 11 DotScreen  
	void doReset_DotScreen() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset DotScreen");
		gDotScrCenter = ofVec2f(0.5);
		gDotScrSize = ofVec2f(1000);
		gDotScrScale = 1.0;
		gDotScrAngle = 1.57;
	};
	// 12 Digital Glitch  
	void doReset_DigitalGlitch() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset DigitalGlitch");
		gGlitchAmount = 0.006;
		gGlitchByp = false;
		gGlitchAngle = 0.02;
		gGlitchSeed = 0.02;
		gGlitchSeedX = 0.02;
		gGlitchSeedY = 0.02;
		gGlitchDistX = 0.02;
		gGlitchDistY = 0.02;
		gGlitchCol = 0.03;
	};
	// 13 Bad TV  
	void doReset_BadTV() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset BadTV");
		gBadTvDist = 3.0;
		gBadTvDist2 = 5.0;
		gBadTvSpeed = 3.0;
		gBadTvRoll = 0.1;
	};
	// 14 Color ACES Filmic  
	void doReset_ACESFilmic() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset ACESFilmic");
		gcolorACESExp = 1.0;
	};
	// 15 Noise Grain 
	void doReset_NoiseGrain() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset NoiseGrain");
		gNoiseAmt = 0.128;
		gNoiseSpeed = 0.08;
	};
	// 16 Tilt Shift Filter 
	void doReset_TiltShif() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset TiltShif");
		gTiltFocus = 0.35;
		gTitltRange = 0.5;
		gTiltOffset = 0.05;
		gTiltStrength = 0.5;
		gTiltFocus = 0.35;
		gTitltRange = 0.5;
		gTiltOffset = 0.05;
		gTiltStrength = 0.5;
	};
	// 17 SuperShader  
	void doReset_SuperShader() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset SuperShader");
		gSupGlowAmt = 0.5;
		gSupGlowSize = 4.0;
		gSupVigOff = 1.0;
		gSupVigDark = 1.0;
		gSupBri = 0.0;
		gSupCont = 0.0;
		gSupSat = 0.0;
		gRGBShfAmt = 0.01;
	};
	// 18 Glitch Automated
	void doReset_GlitchAutomated() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset GlitchAutomated");
		gGliAutoSpeed = 0.6;
		gGliAutoAmt = 0.2;
	};
	// 19 Space Color
	void doReset_SpaceColor() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset SpaceColor");
		gSpaceColorSpeed = 1;
		gSpaceColorOpacity = 0.1;
	};
	// 20 Dither
	void doReset_Dither() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset Dither");
		gDitherScale = 1;
	};
	// 21 Color Invert Strobber
	void doReset_DitherStrobber() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset ColorInvertStrobber");
		gStrobberVolume = 1;
		gStrobberPhase = 0.15;
	};
	// 22 Rimblight Pass
	void doReset_RimblightPass() {
		ofLogNotice("ofxSurfingFxPro") << ("Reset RimblightPass");
		gRimbCol = glm::vec3(2.9, 1.3, 1.3);
		gRimbThres = 64;
	};


public:

	vector<ofxToggle> bEnablers;
	vector<ofParameter<void>> bResets;

	// FXAA
	ofParameterGroup gFxaaGroup;
	ofxFloatSlider gFxaaDivMin;
	ofxFloatSlider gFxaaDivMul;
	ofxFloatSlider gFxaaSpanMax;

	// BLOOM
	ofParameterGroup gBloomGroup;
	ofxFloatSlider gBloomBlurX, gBloomBlurY;

	// DOF
	/*
	ofParameterGroup gDofApertureGroup;
	ofxFloatSlider gDofAperture, gDofMaxBlur, gDofFocus;
	*/

	// GODRAYS
	ofParameterGroup gGodRaysGroup;
	ofxFloatSlider gGodRaysLightDotView;
	ofxVec3Slider gLightPositionOnScreen;

	// SSAO
	ofParameterGroup gSsaoGroup;
	ofxFloatSlider gSsaoAoClamp, gSsaoLumInfluence, gSsaocameraNear, gSsaocameraFar, gSsaofogNear, gSsaofogFar;
	ofxToggle gSsaofogEnabled, gSsaoonlyAO;

	// ZOOM BLUR
	ofParameterGroup gZoomBlurGroup;
	ofxVec2Slider gZoomCenterXY;
	ofxFloatSlider gZoomClamp, gZoomDecay, gZoomWeight, gZoomDensity, gZoomExposure;

	// KALEIDOSCOPE
	ofParameterGroup gKaliGroup;
	ofxFloatSlider gKaleiSegments;

	// RGB SHIFT PASS
	ofParameterGroup gRGBGroup;
	ofxFloatSlider gRGBAmount, gRGBAngle;

	// FILM GRAIN
	ofParameterGroup gFilmGrainGroup;
	ofxFloatSlider gFilmGrainLnIntensity, gFilmGrainLsIntensity, gFilmGrainLCount;
	ofxToggle gFilmGrainLGrayScale;

	// DOT SCREEN
	ofParameterGroup gDotScreenGroup;
	ofxFloatSlider gDotScrAngle, gDotScrScale;
	ofxVec2Slider gDotScrSize, gDotScrCenter;

	// GLITCH
	ofParameterGroup gGlicthGroup;
	ofxFloatSlider gGlitchAmount, gGlitchAngle, gGlitchSeed, gGlitchSeedX, gGlitchSeedY, gGlitchDistX, gGlitchDistY, gGlitchCol;
	ofxToggle gGlitchByp;

	// LAVA FIRE

	// BAD TV PASS
	ofParameterGroup gBadTVGroup;
	ofxFloatSlider gBadTvDist, gBadTvDist2, gBadTvSpeed, gBadTvRoll;

	// colorACES
	ofParameterGroup gcolorACESGroup;
	ofxFloatSlider gcolorACESExp;

	// Noise Grain
	ofParameterGroup gNoiseGroup;
	ofxFloatSlider gNoiseSpeed, gNoiseAmt;

	// TiltShift
	ofParameterGroup gTiltShiftGroup;
	ofxFloatSlider gTiltFocus, gTitltRange, gTiltOffset, gTiltStrength;

	// SuperShader
	ofParameterGroup gSupGroup;
	ofxFloatSlider gSupGlowAmt, gSupGlowSize, gSupVigOff, gSupVigDark, gSupBri, gSupCont, gSupSat, gRGBShfAmt;

	// Glitch Automated
	ofParameterGroup gGliAutoGroup;
	ofxFloatSlider gGliAutoSpeed, gGliAutoAmt;

	// Space Color
	ofParameterGroup gSpaceColorGroup;
	ofxFloatSlider gSpaceColorSpeed, gSpaceColorOpacity;

	// Dither
	ofParameterGroup gDitherGroup;
	ofxFloatSlider gDitherScale;

	// Color Invert Strobber
	ofParameterGroup gStrobberGroup;
	ofxFloatSlider gStrobberVolume;
	ofxFloatSlider gStrobberPhase;

	// SLANT SHIFT PASS
	ofParameterGroup gSlantShiftGroup;
	ofxFloatSlider gSlantShiftVolume;

	// RIMB LIGHT SHIFT
	ofParameterGroup gRimbLightGroup;
	ofxVec3Slider gRimbCol;
	ofxFloatSlider gRimbThres;

	// FOG PASS
	/*ofParameterGroup gFogGroup;
	ofxFloatSlider gFogfogStart, gFogfogEnd;
	ofxVec4Slider gFogfogColor;*/

public:

	ofParameterGroup params;
	ofParameterGroup params_Toggles;
	ofParameterGroup params_Controls;

	//--------------------------------------------------------------
	int getAmountEffectsEnabled()
	{
		int c = 0;
		for (int i = 0; i < post.size(); i++)
		{
			if (bEnablers[i].getParameter().cast<bool>())
			{
				c++;
			}
		}
		return c;
	}

private:

	void Changed_Enablers(ofAbstractParameter& e);

}; 

// 00 Fxaa
// 01 Bloom
// 02 Kaleidoscope
// 03 Noisewarp
// 04 Pixelate
// 05 GodRays
// 06 LimbDarkening
// 07 Ssao
// 08 ZoomBlur
// 09 gRGBShiftPass
// 10 FilmGrainLines
// 11 DotScreen
// 12 Glitch
// 13 Bad TV
// 14 Color ACES Filmic Filter
// 15 Noise Grains Filter
// 16 TiltShift Filter
// 17 SuperShader
// 18 glitch Automated Filter
// 19 gSpaceColor
// 20 Dither Pass
// 21 Color Invert Strobber
// 22 SlantShift Pass
// 23 Rimblight
// 24 FogFilter