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
	void end();

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
	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void savePresetPressed();
	void loadPreset();

public:

	ofParameter<void> btnLoad = { "Load" };
	ofParameter<void> btnSave = { "Save" };

	ofParameter<void> bNone = { "None" };
	ofParameter<void> bAll = { "All" };

private:

	void Changed_bNone();
	void Changed_bAll();

	string fileName;

public:
//private:

	vector<ofxToggle> bEnablers;

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
