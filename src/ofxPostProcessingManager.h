#pragma once

//#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxDC_Utilities.h"
#include "ofxGui.h"

class ofxPostProcessingManager{

public:

    void setup(int w, int h);

    void updateValues();

    void drawGui(int x, int y);
    void drawGui();
    void hideGui();
    void setupGui();

    void begin();
    void begin(ofCamera& cam);

    void end();

    void windowResized(int w, int h);

    void saveSettings(string fileName);
    void loadSettings(string fileName = "");

    // DISABLE ALL EFFECTS
    void disableAll();

    // SWITCH BETWEEN EFFECTS
    void switchFX(int postId);

    // GETTER & SETTERS
    int getEffectNum();
    int getGUIWidth();
    glm::vec2 getGUIPos();

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

    void reInit();

    ofxPostProcessing post;

    //gui------------------------------------------

    ofxPanel gui;
    ofxToggle gDebugDraw;


    // LOAD SeTTINGS
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void savePresetPressed();
    void loadPreset();
    ofParameter<void> btnLoad = {"LOAD SETTINGS"};
    ofParameter<void> btnSave = {"SAVE PRESET"};
    //ofParameter<string> btnFileName = {"PRESET NAME", "..."};
    ofParameter<void> gdisableAll = {"DISABLE ALL FX"};
    void gdisableAllHandler();

    vector<ofxToggle> gBtnEffSwtich;
    void gBtnEffSwtichHandler(bool & val);

    string fileName;


    // FXAA
    ofParameterGroup gFxaaGroup;
    ofxFloatSlider gFxaaDivMin;
    ofxFloatSlider gFxaaDivMul;
    ofxFloatSlider gFxaaSpanMax;

    // BLOOM
    ofParameterGroup gBloomGroup;
    ofxFloatSlider gBloomBlurX, gBloomBlurY;

    // DOF
    /*ofParameterGroup gDofApertureGroup;
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
    ofxFloatSlider gFilmGrainLnIntensity,gFilmGrainLsIntensity, gFilmGrainLCount;
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
    ofxFloatSlider gBadTvDist,gBadTvDist2,gBadTvSpeed,gBadTvRoll;

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
    ofxFloatSlider gGliAutoSpeed,gGliAutoAmt;

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
};


//0-Fxaa
//1-Bloom
//2-Kaleidoscope
//3-Noisewarp
//4-Pixelate
//5-GodRays
//6-LimbDarkening
//7-Ssao
//8-ZoomBlur
//9-gRGBShiftPass
//10-FilmGrainLines
//11-DotScreen
//12-Glitch
//13-Bad TV
//14-Color ACES Filmic Filter
//15-Noise Grains Filter
//16-TiltShift Filter
//17-SuperShader
//18-glitch Automated Filter
//19-gSpaceColor
//20-Dither Pass
//21-Color Invert Strobber
//22-SlantShift Pass
//23-Rimblight
//24-FogFilter
