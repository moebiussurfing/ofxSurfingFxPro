//
//  FilmGrainLinesPass.hpp
//  example_AV_Template
//
//  Created by alp tugan on 4.07.2020.
//

#ifndef FilmGrainLinesPass_hpp
#define FilmGrainLinesPass_hpp

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    /*
     * @see https://github.com/felixturner/bad-tv-shader/blob/master/example/lib/shaders/FilmShader.js
     
     uniforms: {
     
     "tDiffuse":   { type: "t", value: null },
     "time":       { type: "f", value: 0.0 },
     "nIntensity": { type: "f", value: 0.5 },
     "sIntensity": { type: "f", value: 0.05 },
     "sCount":     { type: "f", value: 4096 },
     "grayscale":  { type: "i", value: 1 }
     
     },
     
     */
    class FilmGrainLinesPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<FilmGrainLinesPass> Ptr;
        
        FilmGrainLinesPass(const ofVec2f& aspect, bool arb, float time = 0.0, float nIntensity = 0.5, float sIntensity = 0.5, float sCount = 1096, float grayscale = 1.0);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        float getnIntensity(){ return nIntensity; }
        void setnIntensity(float v) { nIntensity = v; }
        
        float getsIntensity() { return sIntensity; }
        void setsIntensity(float v) { sIntensity = v; }
        
        float getCount() { return sCount; }
        void setCount(float v) { sCount = v; }
        
        void setGrayScale(float v) {grayscale = v;}
        float getGrayScale() {return grayscale;}
    private:
        
        ofShader shader;
        
        float nIntensity, sIntensity, sCount, grayscale, time;
        
    };
}


#endif /* FilmGrainLinesPass_hpp */
