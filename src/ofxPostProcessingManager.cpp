
#include "ofxPostProcessingManager.h"

//--------------------------------------------------------------
ofxPostProcessingManager::ofxPostProcessingManager() {
}

//--------------------------------------------------------------
ofxPostProcessingManager::~ofxPostProcessingManager() {
	exit();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::processOpenFileSelection(ofFileDialogResult openFileResult) {
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "getName(): " + openFileResult.getName();
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "getPath(): " + openFileResult.getPath();

	ofFile file(openFileResult.getPath());

	if (file.exists()) {

		ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "The file exists - now checking the type via file extension";
		string fileExtension = ofToUpper(file.getExtension());

		//We only want images
		if (fileExtension == "JSON") {
			loadSettings(openFileResult.getPath());
		}
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::loadPreset() {
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select fxSettings .json file");

	// Check if the user opened a file
	if (openFileResult.bSuccess) {

		ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "User selected a file";

		// We have a file, check it and process it
		processOpenFileSelection(openFileResult);
	}
	else {
		ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "User hit cancel";
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::savePresetPressed() {
	// Open the Open File Dialog
	ofFileDialogResult saveFileResult = ofSystemSaveDialog("fxSettings_" + ofGetTimestampString() + ".json", "Save your file");
	if (saveFileResult.bSuccess) {
		ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "FX Preset Saved As " << saveFileResult.filePath;
		gui.saveToFile(saveFileResult.filePath);
	}
}

//--------------------------------------------------------------
int ofxPostProcessingManager::getAmountEffects() {
	return post.getPasses().size();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::saveSettings(std::string fileName) {
	gui.saveToFile(fileName);
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "------------------------------------------------";
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "Settings saved as " << fileName;
}

//--------------------------------------------------------------
void ofxPostProcessingManager::loadSettings(std::string fileName) {
	if (fileName == "") {
		fileName = this->fileName;
		gui.loadFromFile(this->fileName);
	}
	else {
		gui.loadFromFile(fileName);
	}
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "------------------------------------------------";
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << "Settings loaded as " << fileName;
}

//--------------------------------------------------------------
void ofxPostProcessingManager::setup(int w, int h) {

	post.init(w, h);

	initializeFX();

	//---------

	setupGui();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::initializeFX() {

	// 2 dof
	//dof = post.createPass<DofPass>();

	// 6 EdgePass
	//post.createPass<EdgePass>()->setEnabled(false);

	// 7 VerticalTilt
	//post.createPass<VerticalTiltShifPass>()->setEnabled(false);

	// 16 Fire Lava
	//lavaFire = post.createPass<FirePass>();

	//gDoEdgePass = false;
	//gDoVerticalTiltShift = false;

	//--

	// 0 Fxaa
	fxaa = post.createPass<FxaaPass>();

	// 1 Bloom
	bloomPass = post.createPass<BloomPass>();

	// 2 Kaleidoscope
	kaleidoscope = post.createPass<KaleidoscopePass>();

	// 3 Noisewarp
	noiseWrap = post.createPass<NoiseWarpPass>();

	// 4 Pixelate
	pixelate = post.createPass<PixelatePass>();

	// 5 GodRays
	godRays = post.createPass<GodRaysPass>();

	// 6 LimbDarkening
	limbDarkening = post.createPass<LimbDarkeningPass>();

	// 7 Ssao
	ssao = post.createPass<SSAOPass>();

	// 8 ZoomBlur
	zoomBlur = post.createPass<ZoomBlurPass>();

	// 9 gRGBShiftPass
	rgbPass = post.createPass<RGBShiftPass>();

	// 10 FilmGrainLines
	filmGrainLinesPass = post.createPass<FilmGrainLinesPass>();

	// 11 DotScreen
	dotScreenPass = post.createPass<DotScreenPass>();

	// 12 Glitch
	glitchPass = post.createPass<DigitalGlitchPass>();

	// 13 Bad TV
	badTv = post.createPass<BadTVPass>();

	// 14 Color ACES Filmic Filter
	colorACES = post.createPass<ACESFilmicToneMappingPass>();

	// 15 Noise Grains Filter
	noiseGrain = post.createPass<NoisePass>();

	// 16 TiltShift Filter
	tiltShift = post.createPass<TiltShiftPass>();

	// 17 SuperShader
	superShader = post.createPass<SuperPass>();

	// 18 glitch Automated Filter
	glitchAuto = post.createPass<GlitchAutomatedPass>();

	// 19 gSpaceColor
	spaceColor = post.createPass<SpaceColorMovePass>();

	// 20 Dither Pass
	dither = post.createPass<DitherPass>();

	// 21 Color Invert Strobber
	strobber = post.createPass<ColorInvert>();

	// 22 Rimblight Pass
	rimbShift = post.createPass<RimHighlightingPass>();

	// 22 SlantShift Pass
	//slantShift = post.createPass<SlantShiftPass>();
	// 24 Fog Filter Pass
	//fogFilter = post.createPass<FogPass>();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doToggleFX(int postId) {
	if (postId >= bEnablers.size()) return;

	bEnablers[postId] = !bEnablers[postId];
	post[postId]->setEnabled(bEnablers[postId]);
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doPowerFX(int postId, bool bState) {
	if (postId >= bEnablers.size()) return;

	bEnablers[postId] = bState;
	post[postId]->setEnabled(bEnablers[postId]);
}

//--------------------------------------------------------------
void ofxPostProcessingManager::updateFX() {

	// 0 FXAA
	if (post[0]->getEnabled()) {
		fxaa->setDivMin(gFxaaDivMin);
		fxaa->setDivMul(gFxaaDivMul);
		fxaa->setSpanMax(gFxaaSpanMax);
	}

	// 1 Bloom
	if (post[1]->getEnabled())
		bloomPass->setBlurXY(gBloomBlurX, gBloomBlurY);

	// 2 Kaleidoscope
	if (post[2]->getEnabled())
		kaleidoscope->setSegments(gKaleiSegments);

	// 3 NoiseWrap
	if (post[3]->getEnabled()) {
	}

	// 4 Pixelate
	if (post[4]->getEnabled()) {

	}

	// 5 GOD Rays
	if (post[5]->getEnabled()) {
		godRays->setLightDirDOTviewDir(gGodRaysLightDotView);
		godRays->setLightPositionOnScreen(gLightPositionOnScreen);
	}

	// 6 Limb Darkening
	if (post[6]->getEnabled()) {

	}

	// 7 SSAO
	if (post[7]->getEnabled()) {
		ssao->setFogEnabled(gSsaofogEnabled);
		ssao->setFogNear(gSsaofogNear);
		ssao->setFogFar(gSsaofogFar);
		ssao->setCameraNear(gSsaocameraNear);
		ssao->setCameraFar(gSsaocameraFar);
		ssao->setOnlyAO(gSsaoonlyAO);
		ssao->setAoClamp(gSsaoAoClamp);
		ssao->setLumInfluence(gSsaoLumInfluence);
	}

	// 8 ZoomBlur
	if (post[8]->getEnabled()) {
		zoomBlur->setCenterX(gZoomCenterXY->x);
		zoomBlur->setCenterY(gZoomCenterXY->y);
		//zoomBlur->setClamp(gZoomClamp);
		//zoomBlur->setDecay(gZoomDecay);
		zoomBlur->setWeight(gZoomWeight);
		zoomBlur->setDensity(gZoomDensity);
		zoomBlur->setExposure(gZoomExposure);
	}

	// 9 RGB Pass
	if (post[9]->getEnabled()) {
		rgbPass->setAngle(gRGBAngle);
		rgbPass->setAmount(gRGBAmount);
	}

	// 10 FilmGrainLines Pass
	if (post[10]->getEnabled()) {
		filmGrainLinesPass->setCount(gFilmGrainLCount);
		filmGrainLinesPass->setGrayScale(gFilmGrainLGrayScale);
		filmGrainLinesPass->setnIntensity(gFilmGrainLnIntensity);
		filmGrainLinesPass->setsIntensity(gFilmGrainLsIntensity);
	}

	// 11 DotScreen Pass
	if (post[11]->getEnabled()) {
		dotScreenPass->setSize(ofVec2f(gDotScrSize->x, gDotScrSize->y));
		dotScreenPass->setAgnle(gDotScrAngle);
		dotScreenPass->setScale(gDotScrScale);
		dotScreenPass->setCenter(ofVec2f(gDotScrCenter->x, gDotScrCenter->y));
	}

	// 12 Glitch Pass
	if (post[12]->getEnabled()) {
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

	// 13 Bad TV Effect
	if (post[13]->getEnabled()) {
		badTv->setSpeed(gBadTvSpeed);
		badTv->setRollSpeed(gBadTvRoll);
		badTv->setDistortion(gBadTvDist);
		badTv->setDistortion2(gBadTvDist2);
	}

	// 14 COLOR ACES FILMIC FILTER
	if (post[14]->getEnabled()) {
		colorACES->setExposure(gcolorACESExp);
	}

	// 15 Noise Grain Filter
	if (post[15]->getEnabled()) {
		noiseGrain->setSpeed(gNoiseSpeed);
		noiseGrain->setAmount(gNoiseAmt);
	}

	// 16 Tilt Shift
	if (post[16]->getEnabled()) {
		tiltShift->setFocusPos(gTiltFocus);
		tiltShift->setRange(gTitltRange);
		tiltShift->setOffset(gTiltOffset);
		tiltShift->setStrength(gTiltStrength);
	}

	// 17 SuperShader
	if (post[17]->getEnabled()) {
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
	if (post[18]->getEnabled()) {
		glitchAuto->setSpeed(gGliAutoSpeed);
		glitchAuto->setAmount(gGliAutoAmt);
	}

	// 19 Space Color
	if (post[19]->getEnabled()) {
		spaceColor->setSpeed(gSpaceColorSpeed);
		spaceColor->setOpacity(gSpaceColorOpacity);
	}

	// 20 Dither
	if (post[20]->getEnabled()) {
		dither->setScale(gDitherScale);
	}

	// 21- Color Invert Strobber
	if (post[21]->getEnabled()) {
		if (utils.notifyPerSecond(gStrobberPhase)) {
			gStrobberVolume = 1;
		}
		else {
			gStrobberVolume = 0;
		}
		//strobber->setVolume(gStrobberVolume);

		auto v = gStrobberVolume.getParameter().cast<float>();
		strobber->setVolume(v.get());

	}

	// 22 Rimblight
	if (post[22]->getEnabled()) {
		rimbShift->setThres(gRimbThres);
		rimbShift->setFloatColor(gRimbCol);
	}

	//--

	// 2 DOF
	//if (post[2]->getEnabled())
	//{
	//	dof->setAperture(gDofAperture);
	//	dof->setMaxBlur(gDofMaxBlur);
	//	dof->setFocus(gDofFocus);
	//}

	// 16 Lava Fire Pass
	//if(post[16]->getEnabled()) {
	//}

	// 24
	//if (post[24]->getEnabled()) {
	//	fogFilter->setColor(gFogfogColor);
	//	fogFilter->setStart(gFogfogStart);
	//	fogFilter->setEnd(gFogfogEnd);
	//}

	//if (ofGetKeyPressed('g')) {
	//	gGlitch = !gGlitch;
	//}
}

//---------------------------------------
void ofxPostProcessingManager::drawDebug() {
	if (gDebugDraw) post.debugDraw();
}

//---------------------------------------
void ofxPostProcessingManager::drawGui() {
	gui.draw();
}

//---------------------------------------
void ofxPostProcessingManager::begin() {
	post.begin();
}
//---------------------------------------
void ofxPostProcessingManager::begin(ofCamera& cam) {
	post.begin(cam);
}
//---------------------------------------
void ofxPostProcessingManager::end(bool autoDraw) {
	post.end(autoDraw);
}

//--------------------------------------------------------------
void ofxPostProcessingManager::Changed_Enablers(ofAbstractParameter& e)
{
	string name = e.getName();

	static bool bAttending = false;

	if (bAttending) return;//skip
	int sz = post.size();

	for (int i = 0; i < sz; i++)
	{
		if (name == bEnablers[i].getName())
		{
			bAttending = true;

			bool b = bEnablers[i].getParameter().cast<bool>().get();
			post[i]->setEnabled(b);

			ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << " #" << i << " " << name << " : " << (b ? "ON" : "OFF");

			// put others to false
			if (bSolo.get())
			{
				for (int j = 0; j < sz; j++)
				{
					if (j != i)
					{
						bEnablers[j].getParameter().cast<bool>().set(false);
						post[j]->setEnabled(false);

						ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__) << " #" << j << " " << bEnablers[j].getName() << " : " << "OFF";
					}
				}
			}

			bAttending = false;

			return;//already found which one changed!
		}
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::windowResized(int w, int h) {

	//post.getPasses().clear();
	post.init(w, h);
	//initializeFX();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doEnableNone() {
	if (bSolo)bSolo = false;

	for (int i = 0; i < post.size(); i++) {
		bEnablers[i] = false;
		post[i]->setEnabled(bEnablers[i]);
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doResetAll() {
	//for (int i = 0; i < post.size(); i++) {
	//	post[i]->reset();
	//}

	doReset_FXAA();
	doReset_Bloom();
	doReset_Kaleidoscope();
	doReset_GodRays();
	doReset_Ssao();
	doReset_ZoomBlur();
	doReset_RGB();
	doReset_FilmGrainLines();
	doReset_DotScreen();
	doReset_DigitalGlitch();
	doReset_BadTV();
	doReset_ACESFilmic();
	doReset_NoiseGrain();
	doReset_SuperShader();
	doReset_GlitchAutomated();
	doReset_SpaceColor();
	doReset_Dither();
	doReset_DitherStrobber();
	doReset_RimblightPass();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doEnableAll() {
	if (bSolo)bSolo = false;

	for (int i = 0; i < post.size(); i++) {
		bEnablers[i] = true;
		post[i]->setEnabled(bEnablers[i]);
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::doRefreshFX() {
	ofLogNotice("ofxSurfingFxPro") << (__FUNCTION__);

	for (int i = 0; i < post.size(); i++) {
		post[i]->setEnabled(bEnablers[i]);
	}
}

//--------------------------------------------------------------
void ofxPostProcessingManager::Changed_bNone() {
	doEnableNone();
}
//--------------------------------------------------------------
void ofxPostProcessingManager::Changed_bReset() {
	doResetAll();
}
//--------------------------------------------------------------
void ofxPostProcessingManager::Changed_bAll() {
	doEnableAll();
}

//--------------------------------------------------------------
void ofxPostProcessingManager::setupGui()
{
	fileName = "fxSettings.json";

	gui.setDefaultTextPadding(6);
	gui.setDefaultWidth(220);
	gui.setDefaultHeight(14);

	gui.setup("POST FX", fileName);
	gui.add(gDebugDraw.setup("DEBUG", true));
	gui.add(bAll);
	gui.add(bSolo);
	gui.add(bNone);
	gui.add(bReset);
	gui.add(btnLoad);
	gui.add(btnSave);

	for (int i = 0; i < post.size(); i++)
	{
		bEnablers.push_back(ofxToggle());
		string n = post[i]->getName();
		bEnablers[i].setup(ofToUpper(n), false);

		bResets.push_back(post[i]->bReset);//get reset param from each fx
	}

	string prefixGroup = "";
	//string prefixGroup = " GROUP";

	string ng = "";

	//--

	// 0 Fxaa
	ng = fxaa->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gFxaaGroup.setName(ng);
	gFxaaGroup.add(gFxaaDivMin.setup("Red Min DIV", 128.0, 1.0, 512.0)->getParameter());
	gFxaaGroup.add(gFxaaDivMul.setup("Red Mult DIV", 8.0, 1.0, 128.0)->getParameter());
	gFxaaGroup.add(gFxaaSpanMax.setup("Span Max", 8.0, 1.0, 128.0)->getParameter());
	gFxaaGroup.add(bResets[0]);
	gui.add(&bEnablers[0]);
	gui.add(gFxaaGroup);

	// 1 Bloom
	ng = bloomPass->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gBloomGroup.setName(ng);
	gBloomGroup.add(gBloomBlurX.setup("Blur X", 0.000953125, 0, 0.01)->getParameter());
	gBloomGroup.add(gBloomBlurY.setup("Blur Y", 0.000953125, 0, 0.01)->getParameter());
	gBloomGroup.add(bResets[1]);
	gui.add(&bEnablers[1]);
	gui.add(gBloomGroup);

	// 2 Kaleidoscope
	ng = kaleidoscope->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gKaliGroup.setName(ng);
	gKaliGroup.add(gKaleiSegments.setup("Segments", 2.f, -20, 20)->getParameter());
	gKaliGroup.add(bResets[2]);
	gui.add(&bEnablers[2]);
	gui.add(gKaliGroup);

	// 3 Noisewarp
	gui.add(&bEnablers[3]);

	// 4 Pixelate
	gui.add(&bEnablers[4]);

	// 5 GodRays
	ng = godRays->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gGodRaysGroup.setName(ng);
	gGodRaysGroup.add(gGodRaysLightDotView.setup("Light", 0.3, 0.0, 1.0)->getParameter());
	gGodRaysGroup.add(gLightPositionOnScreen.setup("Pos", glm::vec3(0), glm::vec3(0), glm::vec3(1))->getParameter());
	gGodRaysGroup.add(bResets[5]);
	gui.add(&bEnablers[5]);
	gui.add(gGodRaysGroup);

	// 6 LimbDarkening
	gui.add(&bEnablers[6]);

	// 7 Ssao
	ng = ssao->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gSsaoGroup.setName(ng);
	gSsaoGroup.add(gSsaofogEnabled.setup("ENABLE Fog", false)->getParameter());
	gSsaoGroup.add(gSsaofogNear.setup("Fog Near", 1, 0, 2000)->getParameter());
	gSsaoGroup.add(gSsaofogFar.setup("Fog Far", 1000, 0, 2000)->getParameter());
	gSsaoGroup.add(gSsaocameraNear.setup("Cam Near", 1, 0, 2000)->getParameter());
	gSsaoGroup.add(gSsaocameraFar.setup("Cam Far", 1000, 0, 2000)->getParameter());
	gSsaoGroup.add(gSsaoonlyAO.setup("ENABLE Ao", false)->getParameter());
	gSsaoGroup.add(gSsaoAoClamp.setup("AoClamp", 0.65, 0.0, 1.0)->getParameter());
	gSsaoGroup.add(gSsaoLumInfluence.setup("LumInfluence", 0.25, 0.0, 1.0)->getParameter());
	gSsaoGroup.add(bResets[7]);
	gui.add(&bEnablers[7]);
	gui.add(gSsaoGroup);

	// 8 ZoomBlur
	ng = zoomBlur->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gZoomBlurGroup.setName(ng);
	gZoomBlurGroup.add(gZoomCenterXY.setup("Center XY", glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(1, 1))->getParameter());
	//gui.add(gZoomClamp.setup("Clamp",1,0,1));
	//gui.add(gZoomDecay.setup("Decay",0.9,0,1));
	gZoomBlurGroup.add(gZoomWeight.setup("Weight", 0.25, 0, 1)->getParameter());
	gZoomBlurGroup.add(gZoomDensity.setup("Density", 0.25, 0, 1)->getParameter());
	gZoomBlurGroup.add(gZoomExposure.setup("Exposure", 0.48, 0, 1)->getParameter());
	gZoomBlurGroup.add(bResets[8]);
	gui.add(&bEnablers[8]);
	gui.add(gZoomBlurGroup);

	// 9 RGB 
	ng = rgbPass->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gRGBGroup.setName(ng);
	gRGBGroup.add(gRGBAngle.setup("Angle", 0, 0, TWO_PI)->getParameter());
	gRGBGroup.add(gRGBAmount.setup("Amount", 0.005, 0, 1)->getParameter());
	gRGBGroup.add(bResets[9]);
	gui.add(&bEnablers[9]);
	gui.add(gRGBGroup);

	// 10 FilmGrainLines  
	ng = filmGrainLinesPass->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gFilmGrainGroup.setName(ng);
	gFilmGrainGroup.add(gFilmGrainLCount.setup("Count", 1096, 1, 10000)->getParameter());
	gFilmGrainGroup.add(gFilmGrainLGrayScale.setup("GrayScale", false)->getParameter());
	gFilmGrainGroup.add(gFilmGrainLnIntensity.setup("N Intensity", 0.5, 0, 1)->getParameter());
	gFilmGrainGroup.add(gFilmGrainLsIntensity.setup("S Intensity", 0.5, 0, 1)->getParameter());
	gFilmGrainGroup.add(bResets[10]);
	gui.add(&bEnablers[10]);
	gui.add(gFilmGrainGroup);

	// 11 DotScreen  
	ng = dotScreenPass->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gDotScreenGroup.setName(ng);
	gDotScreenGroup.add(gDotScrCenter.setup("Center", ofVec2f(0.5), ofVec2f(0), ofVec2f(1.0, 1.0))->getParameter());
	gDotScreenGroup.add(gDotScrSize.setup("Size", ofVec2f(1000), ofVec2f(0), ofVec2f(1000))->getParameter());
	gDotScreenGroup.add(gDotScrScale.setup("Scale", 1.0, 0, 1)->getParameter());
	gDotScreenGroup.add(gDotScrAngle.setup("Angle", 1.57, 0, TWO_PI)->getParameter());
	gDotScreenGroup.add(bResets[11]);
	gui.add(&bEnablers[11]);
	gui.add(gDotScreenGroup);

	// 12 Digital Glitch  
	ng = glitchPass->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gGlicthGroup.setName(ng);
	gGlicthGroup.add(gGlitchAmount.setup("Amount", 0.006, 0.00001, 0.1)->getParameter());
	gGlicthGroup.add(gGlitchByp.setup("Bypass", false)->getParameter());
	gGlicthGroup.add(gGlitchAngle.setup("Angle", 0.02, 0, TWO_PI)->getParameter());
	gGlicthGroup.add(gGlitchSeed.setup("Seed", 0.02, 0, 1)->getParameter());
	gGlicthGroup.add(gGlitchSeedX.setup("Seed X", 0.02, 0, 1)->getParameter());
	gGlicthGroup.add(gGlitchSeedY.setup("Seed Y", 0.02, 0, 1)->getParameter());
	gGlicthGroup.add(gGlitchDistX.setup("Distort X", 0.02, -1, 1)->getParameter());
	gGlicthGroup.add(gGlitchDistY.setup("Distort Y", 0.02, -1, 1)->getParameter());
	gGlicthGroup.add(gGlitchCol.setup("Column", 0.03, 0, 1)->getParameter());
	gGlicthGroup.add(bResets[12]);
	gui.add(&bEnablers[12]);
	gui.add(gGlicthGroup);

	// 13 Bad TV 
	ng = badTv->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gBadTVGroup.setName(ng);
	gBadTVGroup.add(gBadTvDist.setup("Distort 1", 3.0, 0.0, 20.0)->getParameter());
	gBadTVGroup.add(gBadTvDist2.setup("Distort 2", 5.0, 0.0, 20.0)->getParameter());
	gBadTVGroup.add(gBadTvSpeed.setup("Speed", 3.0, 0.0, 20.0)->getParameter());
	gBadTVGroup.add(gBadTvRoll.setup("Roll Speed", 0.1, 0.0, 1.0)->getParameter());
	gBadTVGroup.add(bResets[13]);
	gui.add(&bEnablers[13]);
	gui.add(gBadTVGroup);

	// 14 Color ACES Filmic Filter 
	ng = colorACES->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gcolorACESGroup.setName(ng);
	gcolorACESGroup.add(gcolorACESExp.setup("Exposure", 1.0, 0.0, 1.0)->getParameter());
	gcolorACESGroup.add(bResets[14]);
	gui.add(&bEnablers[14]);
	gui.add(gcolorACESGroup);

	// 15 Noise Grain Filter 
	ng = noiseGrain->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gNoiseGroup.setName(ng);
	gNoiseGroup.add(gNoiseAmt.setup("Amount", 0.128, 0, 1)->getParameter());
	gNoiseGroup.add(gNoiseSpeed.setup("Speed", 0.08, 0, 1)->getParameter());
	gNoiseGroup.add(bResets[15]);
	gui.add(&bEnablers[15]);
	gui.add(gNoiseGroup);

	// 16 Tilt Shift Filter 
	ng = tiltShift->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gTiltShiftGroup.setName(ng);
	gTiltShiftGroup.add(gTiltFocus.setup("Focus", 0.35, 0, 1)->getParameter());
	gTiltShiftGroup.add(gTitltRange.setup("Range", 0.5, 0, 1)->getParameter());
	gTiltShiftGroup.add(gTiltOffset.setup("Offset", 0.05, 0, 1)->getParameter());
	gTiltShiftGroup.add(gTiltStrength.setup("Strength", 0.5, 0, 1)->getParameter());
	gTiltShiftGroup.add(bResets[16]);
	gui.add(&bEnablers[16]);
	gui.add(gTiltShiftGroup);

	// 17 SuperShader Filter 
	ng = superShader->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gSupGroup.setName(ng);
	gSupGroup.add(gSupGlowAmt.setup("Glow Amount", 0.5, 0., 1.)->getParameter());
	gSupGroup.add(gSupGlowSize.setup("Glow Size", 4.0, 0., 20.)->getParameter());
	gSupGroup.add(gSupVigOff.setup("Vignt Offset", 1.0, 0., 1.)->getParameter());
	gSupGroup.add(gSupVigDark.setup("Vignt Darkness", 1.0, 0., 1.)->getParameter());
	gSupGroup.add(gSupBri.setup("Brightness", 0.0, -1., 1.)->getParameter());
	gSupGroup.add(gSupCont.setup("Contrast", 0.0, -1., 1.)->getParameter());
	gSupGroup.add(gSupSat.setup("Saturation", 0.0, 0., 1.)->getParameter());
	gSupGroup.add(gRGBShfAmt.setup("RGB Shift Amt", 0.01, 0., 1.)->getParameter());
	gSupGroup.add(bResets[17]);
	gui.add(&bEnablers[17]);
	gui.add(gSupGroup);

	// 18 Glitch Automated Filter 
	ng = glitchAuto->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gGliAutoGroup.setName(ng);
	gGliAutoGroup.add(gGliAutoSpeed.setup("Speed", 0.6, 0, 1)->getParameter());
	gGliAutoGroup.add(gGliAutoAmt.setup("Amount", 0.2, 0, 1)->getParameter());
	gGliAutoGroup.add(bResets[18]);
	gui.add(&bEnablers[18]);
	gui.add(gGliAutoGroup);

	// 19 Space Color
	ng = spaceColor->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gSpaceColorGroup.setName(ng);
	gSpaceColorGroup.add(gSpaceColorSpeed.setup("Speed", 1, 0, 5)->getParameter());
	gSpaceColorGroup.add(gSpaceColorOpacity.setup("Opacity", 0.1, 0, 1)->getParameter());
	gSpaceColorGroup.add(bResets[19]);
	gui.add(&bEnablers[19]);
	gui.add(gSpaceColorGroup);

	// 20 Dither
	ng = dither->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gDitherGroup.setName(ng);
	gDitherGroup.add(gDitherScale.setup("Scale", 1, 0, 1)->getParameter());
	gDitherGroup.add(bResets[20]);
	gui.add(&bEnablers[20]);
	gui.add(gDitherGroup);

	// 21 Color Invert Strobber
	ng = strobber->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gStrobberGroup.setName(ng);
	gStrobberGroup.add(gStrobberVolume.setup("Volume", 1, 0, 1)->getParameter());
	gStrobberGroup.add(gStrobberPhase.setup("Phase", 0.15, 0, 1)->getParameter());
	gStrobberGroup.add(bResets[21]);
	gui.add(&bEnablers[21]);
	gui.add(gStrobberGroup);

	// 22 Rimblight Pass
	ng = rimbShift->getName() + prefixGroup;
	ng = ofToUpper(ng);
	gRimbLightGroup.setName(ng);
	gRimbLightGroup.add(gRimbCol.setup("Light Color", glm::vec3(2.9, 1.3, 1.3), glm::vec3(0.0), glm::vec3(3.0))->getParameter());
	gRimbLightGroup.add(gRimbThres.setup("Intensity", 64, 0, 512)->getParameter());
	gRimbLightGroup.add(bResets[22]);
	gui.add(&bEnablers[22]);
	gui.add(gRimbLightGroup);

	//--

	/*
	//// 2 DOF
	//gDofApertureGroup.setName(dof->getName() + prefixGroup);
	//gDofApertureGroup.add(gDofFocus.setup("Dof-Focus", 0.9, 0.0, 1.0)->getParameter());
	//gDofApertureGroup.add(gDofAperture.setup("Dof-Aperture", 0.2, 0.0, 1.0)->getParameter());
	//gDofApertureGroup.add(gDofMaxBlur.setup("Dof-MaxBlur", 0.05, 0.0, 1.0)->getParameter());
	////gui.add(gDoDof.setup(dof->getName(), false));
	//gui.add(&bEnablers[2]);
	//gui.add(gDofApertureGroup);

	//gui.add(gDoEdgePass.setup("EdgePass", false));

	//gui.add(gDoVerticalTiltShift.setup("VerticalTiltShift", false));

	// 16 Lava Fire

	//--

	//// 24 Fog Filter
	//gFogGroup.setName(fogFilter->getName() + prefixGroup);
	//gFogGroup.add(gFogfogStart.setup("Fog Start", 0, 0, 2000)->getParameter());
	//gFogGroup.add(gFogfogEnd.setup("Fog End", 10, 0, 2000)->getParameter());
	//gFogGroup.add(gFogfogColor.setup("Fog Color", glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 0), glm::vec4(1, 1, 1, 1))->getParameter());
	//gui.add(&bEnablers[24]);
	//gui.add(gFogGroup);
	*/

	//--

	// Reset Callbacks
	listeners_bReset.unsubscribeAll();
	listeners_bReset.push(bResets[0].newListener([this]() { doReset_FXAA(); }));
	listeners_bReset.push(bResets[1].newListener([this]() { doReset_Bloom(); }));
	listeners_bReset.push(bResets[2].newListener([this]() { doReset_Kaleidoscope(); }));
	listeners_bReset.push(bResets[5].newListener([this]() { doReset_GodRays(); }));
	listeners_bReset.push(bResets[7].newListener([this]() { doReset_Ssao(); }));
	listeners_bReset.push(bResets[8].newListener([this]() { doReset_ZoomBlur(); }));
	listeners_bReset.push(bResets[9].newListener([this]() { doReset_RGB(); }));
	listeners_bReset.push(bResets[10].newListener([this]() { doReset_FilmGrainLines(); }));
	listeners_bReset.push(bResets[11].newListener([this]() { doReset_DotScreen(); }));
	listeners_bReset.push(bResets[12].newListener([this]() { doReset_DigitalGlitch(); }));
	listeners_bReset.push(bResets[13].newListener([this]() { doReset_BadTV(); }));
	listeners_bReset.push(bResets[14].newListener([this]() { doReset_ACESFilmic(); }));
	listeners_bReset.push(bResets[15].newListener([this]() { doReset_NoiseGrain(); }));
	listeners_bReset.push(bResets[16].newListener([this]() { doReset_TiltShif(); }));
	listeners_bReset.push(bResets[17].newListener([this]() { doReset_SuperShader(); }));
	listeners_bReset.push(bResets[18].newListener([this]() { doReset_GlitchAutomated(); }));
	listeners_bReset.push(bResets[19].newListener([this]() { doReset_SpaceColor(); }));
	listeners_bReset.push(bResets[20].newListener([this]() { doReset_Dither(); }));
	listeners_bReset.push(bResets[21].newListener([this]() { doReset_DitherStrobber(); }));
	listeners_bReset.push(bResets[22].newListener([this]() { doReset_RimblightPass(); }));

	//--

	gui.minimizeAll();

	bAll.addListener(this, &ofxPostProcessingManager::Changed_bAll);
	bNone.addListener(this, &ofxPostProcessingManager::Changed_bNone);
	bReset.addListener(this, &ofxPostProcessingManager::Changed_bReset);

	btnLoad.addListener(this, &ofxPostProcessingManager::loadPreset);
	btnSave.addListener(this, &ofxPostProcessingManager::savePresetPressed);

	doEnableNone();

	//--

	// Toggles

	params_Toggles.setName("Toggles");
	for (int i = 0; i < post.size(); i++)
	{
		params_Toggles.add(bEnablers[i].getParameter());
	}

	ofAddListener(params_Toggles.parameterChangedE(), this, &ofxPostProcessingManager::Changed_Enablers);

	//--

	// Controls

	params_Controls.setName("Controls");
	params_Controls.add(gFxaaGroup);		// 0
	params_Controls.add(gBloomGroup);		// 1
	params_Controls.add(gKaliGroup);		// 2
	params_Controls.add(gGodRaysGroup);		// 3
	params_Controls.add(gSsaoGroup);		// 4
	params_Controls.add(gZoomBlurGroup);	// 5
	params_Controls.add(gRGBGroup);			// 6
	params_Controls.add(gFilmGrainGroup);	// 7
	params_Controls.add(gDotScreenGroup);	// 8
	params_Controls.add(gGlicthGroup);		// 9
	params_Controls.add(gBadTVGroup);		// 10
	params_Controls.add(gcolorACESGroup);	// 11
	params_Controls.add(gNoiseGroup);		// 12
	params_Controls.add(gTiltShiftGroup);	// 13
	params_Controls.add(gSupGroup);			// 14
	params_Controls.add(gGliAutoGroup);		// 15
	params_Controls.add(gSpaceColorGroup);	// 16
	params_Controls.add(gDitherGroup);		// 17
	params_Controls.add(gStrobberGroup);	// 18
	params_Controls.add(gRimbLightGroup);	// 19

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

	//--

	// All

	params.setName("FX PRO");
	params.add(gui.getParameter());
}

//---------------------------------------
void ofxPostProcessingManager::exit()
{
	ofRemoveListener(params_Toggles.parameterChangedE(), this, &ofxPostProcessingManager::Changed_Enablers);

	bAll.removeListener(this, &ofxPostProcessingManager::Changed_bAll);
	bNone.removeListener(this, &ofxPostProcessingManager::Changed_bNone);
	bReset.removeListener(this, &ofxPostProcessingManager::Changed_bReset);

	btnLoad.removeListener(this, &ofxPostProcessingManager::loadPreset);
	btnSave.removeListener(this, &ofxPostProcessingManager::savePresetPressed);
}