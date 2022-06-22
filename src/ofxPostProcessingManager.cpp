
#include "ofxPostProcessingManager.h"

void ofxPostProcessingManager::processOpenFileSelection(ofFileDialogResult openFileResult) {
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());

    ofFile file (openFileResult.getPath());

    if (file.exists()){

        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());

        //We only want images
        if (fileExtension == "JSON") {
            loadSettings(openFileResult.getPath());
        }
    }
}

void ofxPostProcessingManager::loadPreset() {
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select fxSettings .json file");

    //Check if the user opened a file
    if (openFileResult.bSuccess){

        ofLogVerbose("User selected a file");

        //We have a file, check it and process it
        processOpenFileSelection(openFileResult);

    }else {
        ofLogVerbose("User hit cancel");
    }
}

void ofxPostProcessingManager::savePresetPressed() {
    //Open the Open File Dialog
    ofFileDialogResult saveFileResult = ofSystemSaveDialog("fxSettings_"+ofGetTimestampString() + ".json", "Save your file");
    if (saveFileResult.bSuccess){
        ofLog() << "FX Preset Saved As " << saveFileResult.filePath;
        gui.saveToFile(saveFileResult.filePath);
    }

}

int ofxPostProcessingManager::getEffectNum() {
    return post.getPasses().size();
}

void ofxPostProcessingManager::saveSettings(std::string fileName) {
    gui.saveToFile(fileName);
    ofLog() << "------------------------------------------------";
    ofLog() << "Settings saved as " << fileName;
}


void ofxPostProcessingManager::loadSettings(std::string fileName) {
    if(fileName == "") {
        fileName = this->fileName;
        gui.loadFromFile(this->fileName);
    }else{
        gui.loadFromFile(fileName);
    }
    ofLog() << "------------------------------------------------";
    ofLog() << "Settings loaded as " << fileName;
}

//---------------------------------------
void ofxPostProcessingManager::setup(int w, int h){

    post.init(w, h);

    reInit();

    //---------
    setupGui();
}

void ofxPostProcessingManager::reInit() {

    //2-dof
    //dof = post.createPass<DofPass>();

    //6-EdgePass
    //post.createPass<EdgePass>()->setEnabled(false);

    //7-VerticalTilt
    //post.createPass<VerticalTiltShifPass>()->setEnabled(false);

    //16-Fire Lava
    //lavaFire = post.createPass<FirePass>();

    /*gDoEdgePass = false;
     gDoVerticalTiltShift = false;*/


    //0-Fxaa
    fxaa = post.createPass<FxaaPass>();

    //1-Bloom
    bloomPass = post.createPass<BloomPass>();

    //2-Kaleidoscope
    kaleidoscope = post.createPass<KaleidoscopePass>();

    //3-Noisewarp
    noiseWrap = post.createPass<NoiseWarpPass>();


    //4-Pixelate
    pixelate = post.createPass<PixelatePass>();

    //5-GodRays
    godRays = post.createPass<GodRaysPass>();

    //6-LimbDarkening
    limbDarkening=post.createPass<LimbDarkeningPass>();

    //7-Ssao
    ssao=post.createPass<SSAOPass>();

    //8-ZoomBlur
    zoomBlur = post.createPass<ZoomBlurPass>();

    //9-gRGBShiftPass
    rgbPass = post.createPass<RGBShiftPass>();

    //10-FilmGrainLines
    filmGrainLinesPass = post.createPass<FilmGrainLinesPass>();

    //11-DotScreen
    dotScreenPass = post.createPass<DotScreenPass>();

    //12-Glitch
    glitchPass = post.createPass<DigitalGlitchPass>();

    //13-Bad TV
    badTv = post.createPass<BadTVPass>();

    //14-Color ACES Filmic Filter
    colorACES = post.createPass<ACESFilmicToneMappingPass>();

    //15-Noise Grains Filter
    noiseGrain = post.createPass<NoisePass>();

    //16-TiltShift Filter
    tiltShift = post.createPass<TiltShiftPass>();

    //17-SuperShader
    superShader = post.createPass<SuperPass>();

    //18-glitch Automated Filter
    glitchAuto = post.createPass<GlitchAutomatedPass>();

    //19-gSpaceColor
    spaceColor = post.createPass<SpaceColorMovePass>();

    //20-Dither Pass
    dither = post.createPass<DitherPass>();

    //21-Color Invert Strobber
    strobber = post.createPass<ColorInvert>();

    // 22-Rimblight Pass
    rimbShift = post.createPass<RimHighlightingPass>();

    //22-SlantShift Pass
    //slantShift = post.createPass<SlantShiftPass>();
    // 24-Fog Filter Pass
    //fogFilter = post.createPass<FogPass>();
}
//---------------------------------------
// SWTICH EFFECT
//---------------------------------------
void ofxPostProcessingManager::switchFX(int postId) {
    gBtnEffSwtich[postId] = !gBtnEffSwtich[postId];
    post[postId]->setEnabled(gBtnEffSwtich[postId]);
}

//---------------------------------------
// UPDATE EFFECT PARAMETERS
//---------------------------------------
void ofxPostProcessingManager::updateValues(){

    // 2- DOF
    /*if(post[2]->getEnabled())
     {
     dof->setAperture(gDofAperture);
     dof->setMaxBlur(gDofMaxBlur);
     dof->setFocus(gDofFocus);
     }*/



    // 16- Lava Fire Pass
    //if(post[16]->getEnabled()) {
    //}
    // 0 - FXAA
    if(post[0]->getEnabled()) {
        fxaa->setDivMin(gFxaaDivMin);
        fxaa->setDivMul(gFxaaDivMul);
        fxaa->setSpanMax(gFxaaSpanMax);
    }

    // 1- Bloom
    if(post[1]->getEnabled())
        bloomPass->setBlurXY(gBloomBlurX, gBloomBlurY);

    // 2- Kaleidoscope
    if(post[2]->getEnabled())
        kaleidoscope->setSegments(gKaleiSegments);

    // 3- NoiseWrap
    if(post[3]->getEnabled()) {

    }

    // 4- Pixelate
    if(post[4]->getEnabled()) {

    }

    // 5- GOD Rays
    if(post[5]->getEnabled()) {
        godRays->setLightDirDOTviewDir(gGodRaysLightDotView);
        godRays->setLightPositionOnScreen(gLightPositionOnScreen);
    }

    // 6- Limb Darkening
    if(post[6]->getEnabled()) {

    }

    // 7- SSAO
    if(post[7]->getEnabled()) {
        ssao->setFogEnabled(gSsaofogEnabled);
        ssao->setFogNear(gSsaofogNear);
        ssao->setFogFar(gSsaofogFar);
        ssao->setCameraNear(gSsaocameraNear);
        ssao->setCameraFar(gSsaocameraFar);
        ssao->setOnlyAO(gSsaoonlyAO);
        ssao->setAoClamp(gSsaoAoClamp);
        ssao->setLumInfluence(gSsaoLumInfluence);
    }

    // 8- ZoomBlur
    if(post[8]->getEnabled()) {
        zoomBlur->setCenterX(gZoomCenterXY->x);
        zoomBlur->setCenterY(gZoomCenterXY->y);
        //zoomBlur->setClamp(gZoomClamp);
        //zoomBlur->setDecay(gZoomDecay);
        zoomBlur->setWeight(gZoomWeight);
        zoomBlur->setDensity(gZoomDensity);
        zoomBlur->setExposure(gZoomExposure);
    }

    // 9- RGB Pass
    if(post[9]->getEnabled()) {
        rgbPass->setAngle(gRGBAngle);
        rgbPass->setAmount(gRGBAmount);
    }

    // 10- FilmGrainLines Pass
    if(post[10]->getEnabled()) {
        filmGrainLinesPass->setCount(gFilmGrainLCount);
        filmGrainLinesPass->setGrayScale(gFilmGrainLGrayScale);
        filmGrainLinesPass->setnIntensity(gFilmGrainLnIntensity);
        filmGrainLinesPass->setsIntensity(gFilmGrainLsIntensity);
    }

    // 11- DotScreen Pass
    if(post[11]->getEnabled())  {
        dotScreenPass->setSize(ofVec2f(gDotScrSize->x, gDotScrSize->y));
        dotScreenPass->setAgnle(gDotScrAngle);
        dotScreenPass->setScale(gDotScrScale);
        dotScreenPass->setCenter(ofVec2f(gDotScrCenter->x, gDotScrCenter->y));
    }

    // 12- Glitch Pass
    if(post[12]->getEnabled()) {
        gGlitchCol = ofRandom(0.001, 1);
        gGlitchDistX = ofSignedNoise(ofGetFrameNum() * 0.04);
        gGlitchDistY = ofSignedNoise(ofGetFrameNum() * 0.01);
        gGlitchAngle = ofRandom(0, TWO_PI);

        //glitchPass->setByp(gGlitchByp);
        glitchPass->setCols(gGlitchCol);
        glitchPass->setSeed(gGlitchSeed);
        glitchPass->setSeedX(gGlitchSeedX);
        glitchPass->setSeedY(gGlitchSeedY);
        glitchPass->setDistortionX(gGlitchDistX);
        glitchPass->setDistortionY(gGlitchDistY);
        glitchPass->setAngle(gGlitchAngle);
        glitchPass->setAmount(gGlitchAmount);
    }

    // 13- Bad TV Effect
    if(post[13]->getEnabled()) {
        badTv->setSpeed(gBadTvSpeed);
        badTv->setRollSpeed(gBadTvRoll);
        badTv->setDistortion(gBadTvDist);
        badTv->setDistortion2(gBadTvDist2);
    }

    // 14- COLOR ACES FILMIC FILTER
    if(post[14]->getEnabled()) {
        colorACES->setExposure(gcolorACESExp);
    }

    // 15- Noise Grain Filter
    if(post[15]->getEnabled()) {
        noiseGrain->setSpeed(gNoiseSpeed);
        noiseGrain->setAmount(gNoiseAmt);
    }

    // 16- Tilt Shift
    if(post[16]->getEnabled()) {
        tiltShift->setFocusPos(gTiltFocus);
        tiltShift->setRange(gTitltRange);
        tiltShift->setOffset(gTiltOffset);
        tiltShift->setStrength(gTiltStrength);
    }

    // 17- SuperShader
    if(post[17]->getEnabled()) {
        superShader->setGlowAmount(gSupGlowAmt);
        superShader->setGlowSize(gSupGlowSize);
        superShader->setVigOffset(gSupVigOff);
        superShader->setVigDark(gSupVigDark);
        superShader->setBrightness(gSupBri);
        superShader->setContrast(gSupCont);
        superShader->setSaturation(gSupSat);
        superShader->setRGBShiftAmount(gRGBShfAmt);
    }

    // 18 Automated Glitch
    if(post[18]->getEnabled()) {
        glitchAuto->setSpeed(gGliAutoSpeed);
        glitchAuto->setAmount(gGliAutoAmt);
    }

    // 19 Space Color
    if(post[19]->getEnabled()) {
        spaceColor->setSpeed(gSpaceColorSpeed);
        spaceColor->setOpacity(gSpaceColorOpacity);
    }

    // 20- Dither
    if(post[20]->getEnabled()) {
        dither->setScale(gDitherScale);
    }

    // 21- Color Invert Strobber
    if(post[21]->getEnabled()) {
        if(utils.notifyPerSecond(gStrobberPhase)) {
            gStrobberVolume = 1;
        }else{
            gStrobberVolume = 0;
        }
        strobber->setVolume(gStrobberVolume);
    }

    // 22- Rimblight
    if(post[22]->getEnabled()) {
        rimbShift->setThres(gRimbThres);
        rimbShift->setFloatColor(gRimbCol);
    }

    // 24
    /*if(post[24]->getEnabled()) {
        fogFilter->setColor(gFogfogColor);
        fogFilter->setStart(gFogfogStart);
        fogFilter->setEnd(gFogfogEnd);
    }*/

    /*if(ofGetKeyPressed('g')) {
     gGlitch = !gGlitch;
     }*/
}


//---------------------------------------
void ofxPostProcessingManager::drawGui(){

    if(gDebugDraw)
        post.debugDraw();

    gui.draw();
}

//---------------------------------------
void ofxPostProcessingManager::drawGui(int x, int y){

    gui.setPosition(x, y);

    if(gDebugDraw)
        post.debugDraw();

    gui.draw();
}

void ofxPostProcessingManager::hideGui() {
    gui.setPosition(0, 2500);

    gui.draw();
}
//---------------------------------------
void ofxPostProcessingManager::begin(){
    post.begin();
}
//---------------------------------------
void ofxPostProcessingManager::begin(ofCamera& cam){
    post.begin(cam);
}
//---------------------------------------
void ofxPostProcessingManager::end() {
    post.end();
}


void ofxPostProcessingManager::gBtnEffSwtichHandler(bool & val) {
    for (int i = 0; i < post.size(); i++) {
        post[i]->setEnabled(gBtnEffSwtich[i]);
    }
}

void ofxPostProcessingManager::windowResized(int w, int h) {

    //post.getPasses().clear();
    post.init(w,h);
    //reInit();
}

void ofxPostProcessingManager::disableAll() {
    for (int i=0; i < post.size(); i++) {
        gBtnEffSwtich[i] = false;
        post[i]->setEnabled(gBtnEffSwtich[i]);
    }
}

void ofxPostProcessingManager::gdisableAllHandler() {
    disableAll();
}

//---------------------------------------
void ofxPostProcessingManager::setupGui(){
    fileName = "fxSettings.json";
    gui.setDefaultTextPadding(6);
    gui.setDefaultWidth(220);
    gui.setDefaultHeight(14);
    gui.setup("POST PROCESSING FX", fileName);
    gui.add(gDebugDraw.setup("ENABLE DEBUG DRAW", false));
    gui.add(gdisableAll);
    gui.add(btnLoad);
    //gui.add(btnFileName );
    gui.add(btnSave);

    for (int i = 0; i < post.size(); i++) {
        gBtnEffSwtich.push_back(ofxToggle());
        gBtnEffSwtich[i].setup(ofToString(i) + "." + post[i]->getName(), false);
        gBtnEffSwtich[i].addListener(this, &ofxPostProcessingManager::gBtnEffSwtichHandler);
    }

    //2-dof
    /*gDofApertureGroup.setName(dof->getName() + " GROUP");
     gDofApertureGroup.add(gDofFocus.setup("Dof-Focus", 0.9, 0.0, 1.0)->getParameter());
     gDofApertureGroup.add(gDofAperture.setup("Dof-Aperture", 0.2, 0.0, 1.0)->getParameter());
     gDofApertureGroup.add(gDofMaxBlur.setup("Dof-MaxBlur", 0.05, 0.0, 1.0)->getParameter());
     //gui.add(gDoDof.setup(dof->getName(), false));
     gui.add(&gBtnEffSwtich[2]);
     gui.add(gDofApertureGroup);
     */

    //gui.add(gDoEdgePass.setup("EdgePass", false));

    //gui.add(gDoVerticalTiltShift.setup("VerticalTiltShift", false));

    // 16- Lava Fire

    //0-Fxaa
    gFxaaGroup.setName(fxaa->getName() + " GROUP");
    gFxaaGroup.add(gFxaaDivMin.setup("REDUCE MIN DIV", 128.0, 1.0, 512.0 )->getParameter());
    gFxaaGroup.add(gFxaaDivMul.setup("REDUCE MULT DIV", 8.0, 1.0, 128.0 )->getParameter());
    gFxaaGroup.add(gFxaaSpanMax.setup("SPAN MAX", 8.0, 1.0, 128.0 )->getParameter());
    gui.add(&gBtnEffSwtich[0]);
    gui.add(gFxaaGroup);

    //1-Bloom
    gBloomGroup.setName(bloomPass->getName() + " GROUP");
    gBloomGroup.add(gBloomBlurX.setup("Blur X", 0.000953125, 0, 0.01 )->getParameter());
    gBloomGroup.add(gBloomBlurY.setup("Blur Y", 0.000953125, 0, 0.01 )->getParameter());
    gui.add(&gBtnEffSwtich[1]);
    gui.add(gBloomGroup);

    //2-Kaleidoscope
    gKaliGroup.setName(kaleidoscope->getName() + " GROUP");
    gKaliGroup.add(gKaleiSegments.setup("Segments", 2.f, -20, 20)->getParameter());
    gui.add(&gBtnEffSwtich[2]);
    gui.add(gKaliGroup);

    //3-Noisewarp
    gui.add(&gBtnEffSwtich[3]);

    // 4- Pixelate
    gui.add(&gBtnEffSwtich[4]);

    //5-GodRays
    gGodRaysGroup.setName(godRays->getName() + " GROUP");
    gGodRaysGroup.add(gGodRaysLightDotView.setup("GodRays-Light", 0.3, 0.0, 1.0)->getParameter());
    gGodRaysGroup.add(gLightPositionOnScreen.setup("Light Position", glm::vec3(0), glm::vec3(0), glm::vec3(1))->getParameter());
    gui.add(&gBtnEffSwtich[5]);
    gui.add(gGodRaysGroup);

    //6-LimbDarkening
    gui.add(&gBtnEffSwtich[6]);

    //7-Ssao
    gSsaoGroup.setName(ssao->getName() + " GROUP");
    gSsaoGroup.add(gSsaofogEnabled.setup("Enable Fog", false)->getParameter());
    gSsaoGroup.add(gSsaofogNear.setup("Fog Near", 1, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaofogFar.setup("Fog Far", 1000, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaocameraNear.setup("Camera Near",1, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaocameraFar.setup("Camera Far",1000, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaoonlyAO.setup("Enable-Ao", false)->getParameter());
    gSsaoGroup.add(gSsaoAoClamp.setup("Ssao-AoClamp", 0.65, 0.0, 1.0)->getParameter());
    gSsaoGroup.add(gSsaoLumInfluence.setup("Ssao-LumInfluence", 0.25, 0.0, 1.0)->getParameter());
    gui.add(&gBtnEffSwtich[7]);
    gui.add(gSsaoGroup);

    // 8- ZoomBlur
    gZoomBlurGroup.setName(zoomBlur->getName() + " GROUP");
    gZoomBlurGroup.add(gZoomCenterXY.setup("Zoom Center XY", glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(1, 1))->getParameter());
    //gui.add(gZoomClamp.setup("Clamp",1,0,1));
    //gui.add(gZoomDecay.setup("Decay",0.9,0,1));
    gZoomBlurGroup.add(gZoomWeight.setup("Weight",0.25,0,1)->getParameter());
    gZoomBlurGroup.add(gZoomDensity.setup("Density",0.25,0,1)->getParameter());
    gZoomBlurGroup.add(gZoomExposure.setup("Exposure",0.48,0,1)->getParameter());
    gui.add(&gBtnEffSwtich[8]);
    gui.add(gZoomBlurGroup);

    // 9- RGB Pass
    gRGBGroup.setName(rgbPass->getName() + " GROUP");
    gRGBGroup.add(gRGBAngle.setup("Angle", 0, 0, TWO_PI)->getParameter());
    gRGBGroup.add(gRGBAmount.setup("Amount", 0.005,0,1)->getParameter());
    gui.add(&gBtnEffSwtich[9]);
    gui.add(gRGBGroup);

    // 10- FilmGrainLines Pass Params
    gFilmGrainGroup.setName(filmGrainLinesPass->getName() + " GROUP");
    gFilmGrainGroup.add(gFilmGrainLCount.setup("Count", 1096,1, 10000)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLGrayScale.setup("GrayScale",false)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLnIntensity.setup("N Intensity",0.5,0,1)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLsIntensity.setup("S Intensity",0.5,0,1)->getParameter());
    gui.add(&gBtnEffSwtich[10]);
    gui.add(gFilmGrainGroup);


    // 11- DotScreen Pass params
    gDotScreenGroup.setName(dotScreenPass->getName() + " GROUP");
    gDotScreenGroup.add(gDotScrCenter.setup("Center", ofVec2f(0.5), ofVec2f(0), ofVec2f(1.0,1.0))->getParameter());
    gDotScreenGroup.add(gDotScrSize.setup("Size", ofVec2f(1000), ofVec2f(0), ofVec2f(1000))->getParameter());
    gDotScreenGroup.add(gDotScrScale.setup("Scale",1.0,0,1)->getParameter());
    gDotScreenGroup.add(gDotScrAngle.setup("Angle",1.57,0,TWO_PI)->getParameter());
    gui.add(&gBtnEffSwtich[11]);
    gui.add(gDotScreenGroup);


    // 12- Digital Glitch Pass Params
    gGlicthGroup.setName(glitchPass->getName() + " GROUP");
    gGlicthGroup.add(gGlitchAmount.setup("Amount", 0.006, 0.00001, 0.1)->getParameter());
    gGlicthGroup.add(gGlitchByp.setup("Bypass", false)->getParameter());
    gGlicthGroup.add(gGlitchAngle.setup("Angle", 0.02, 0, TWO_PI)->getParameter());
    gGlicthGroup.add(gGlitchSeed.setup("Seed", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchSeedX.setup("Seed X", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchSeedY.setup("Seed Y", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchDistX.setup("Distortion X", 0.02, -1, 1)->getParameter());
    gGlicthGroup.add(gGlitchDistY.setup("Distortion Y", 0.02, -1, 1)->getParameter());
    gGlicthGroup.add(gGlitchCol.setup("Column", 0.03, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[12]);
    gui.add(gGlicthGroup);



    // 13- Bad TV Pass Params
    gBadTVGroup.setName(badTv->getName() + " GROUP");
    gBadTVGroup.add(gBadTvDist.setup("Distortion 1", 3.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvDist2.setup("Distortion 2", 5.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvSpeed.setup("Speed", 3.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvRoll.setup("Roll Speed", 0.1, 0.0, 1.0)->getParameter());
    gui.add(&gBtnEffSwtich[13]);
    gui.add(gBadTVGroup);

    // 14- Color ACES Filmic Filter Paramas
    gcolorACESGroup.setName(colorACES->getName() + " GROUP");
    gcolorACESGroup.add(gcolorACESExp.setup("Exposure", 1.0, 0.0, 1.0)->getParameter());
    gui.add(&gBtnEffSwtich[14]);
    gui.add(gcolorACESGroup);

    // 15- Noise Grain Filter Params
    gNoiseGroup.setName(noiseGrain->getName() + " GROUP");
    gNoiseGroup.add(gNoiseAmt.setup("Amount", 0.128, 0, 1)->getParameter());
    gNoiseGroup.add(gNoiseSpeed.setup("Speed", 0.08, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[15]);
    gui.add(gNoiseGroup);

    // 16- Tilt Shift Filter Params
    gTiltShiftGroup.setName(tiltShift->getName() + " GROUP");
    gTiltShiftGroup.add(gTiltFocus.setup("Focus", 0.35, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTitltRange.setup("Range", 0.5, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTiltOffset.setup("Offset", 0.05, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTiltStrength.setup("Strength", 0.5, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[16]);
    gui.add(gTiltShiftGroup);

    // 17- SuperShader Filter Params
    gSupGroup.setName(superShader->getName() + " GROUP");
    gSupGroup.add(gSupGlowAmt.setup("Glow Amount", 0.5, 0., 1.)->getParameter());
    gSupGroup.add(gSupGlowSize.setup("Glow Size", 4.0, 0., 20.)->getParameter());
    gSupGroup.add(gSupVigOff.setup("Vignette Offset", 1.0, 0., 1.)->getParameter());
    gSupGroup.add(gSupVigDark.setup("Vignette Darkness", 1.0, 0., 1.)->getParameter());
    gSupGroup.add(gSupBri.setup("Brightness", 0.0, -1., 1.)->getParameter());
    gSupGroup.add(gSupCont.setup("Contrast", 0.0, -1., 1.)->getParameter());
    gSupGroup.add(gSupSat.setup("Saturation", 0.0, 0., 1.)->getParameter());
    gSupGroup.add(gRGBShfAmt.setup("RGB Shift Amount", 0.01, 0., 1.)->getParameter());
    gui.add(&gBtnEffSwtich[17]);
    gui.add(gSupGroup);

    // 18- Glitch Automated Filter Params
    gGliAutoGroup.setName(glitchAuto->getName() + " GROUP");
    gGliAutoGroup.add(gGliAutoSpeed.setup("Speed", 0.6, 0 , 1)->getParameter());
    gGliAutoGroup.add(gGliAutoAmt.setup("Amount", 0.2, 0 , 1)->getParameter());
    gui.add(&gBtnEffSwtich[18]);
    gui.add(gGliAutoGroup);

    // 19- Space Color
    gSpaceColorGroup.setName(spaceColor->getName() + " GROUP");
    gSpaceColorGroup.add(gSpaceColorSpeed.setup("Speed", 1, 0, 5)->getParameter());
    gSpaceColorGroup.add(gSpaceColorOpacity.setup("Opacity", 0.1, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[19]);
    gui.add(gSpaceColorGroup);

    // 20- Dither
    gDitherGroup.setName(dither->getName() + " GROUP");
    gDitherGroup.add(gDitherScale.setup("Scale", 1, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[20]);
    gui.add(gDitherGroup);

    // 21- Color Invert Strobber
    gStrobberGroup.setName(strobber->getName() + " GROUP");
    gStrobberGroup.add(gStrobberVolume.setup("Volume", 1, 0, 1)->getParameter());
    gStrobberGroup.add(gStrobberPhase.setup("Phase", 0.15, 0, 1)->getParameter());
    gui.add(&gBtnEffSwtich[21]);
    gui.add(gStrobberGroup);


    // 22- Rimblight Pass
    gRimbLightGroup.setName(rimbShift->getName() + " GROUP");
    gRimbLightGroup.add(gRimbCol.setup("Light Color", glm::vec3(2.9, 1.3, 1.3), glm::vec3(0.0), glm::vec3(3.0))->getParameter());
    gRimbLightGroup.add(gRimbThres.setup("Intensity", 64, 0, 512)->getParameter());
    gui.add(&gBtnEffSwtich[22]);
    gui.add(gRimbLightGroup);


    // 24- Fog Filter Pass
    /*gFogGroup.setName(fogFilter->getName() + " GROUP");
    gFogGroup.add(gFogfogStart.setup("Fog Start", 0, 0, 2000)->getParameter());
    gFogGroup.add(gFogfogEnd.setup("Fog End", 10, 0, 2000)->getParameter());
    gFogGroup.add(gFogfogColor.setup("Fog Color", glm::vec4(0,0,0,1), glm::vec4(0,0,0,0), glm::vec4(1,1,1,1))->getParameter());
    gui.add(&gBtnEffSwtich[24]);
    gui.add(gFogGroup);*/

    // 26-
    gui.minimizeAll();
    gdisableAll.addListener(this, &ofxPostProcessingManager::gdisableAllHandler);
    btnLoad.addListener(this,&ofxPostProcessingManager::loadPreset);
    btnSave.addListener(this,&ofxPostProcessingManager::savePresetPressed);

    gdisableAllHandler();


    //--

    params.setName("ofxPostProcessingManager");
    params.add(gui.getParameter());
}

//---------------------------------------------------------------------------------------
int ofxPostProcessingManager::getGUIWidth() {
    return gui.getWidth();

}

glm::vec2 ofxPostProcessingManager::getGUIPos() {
    glm::vec2 p (gui.getPosition().x, gui.getPosition().y);
    return p;

}
