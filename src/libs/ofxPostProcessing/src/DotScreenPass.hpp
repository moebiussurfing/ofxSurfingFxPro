//
//  DotScreenPass.hpp
//  example_AV_Template
//
//  Created by alp tugan on 5.07.2020.
//
// Translated from https://github.com/hughsk/three-effectcomposer/blob/master/demo.js


#ifndef DotScreenPass_hpp
#define DotScreenPass_hpp


#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class DotScreenPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<DotScreenPass> Ptr;
        
        DotScreenPass(const ofVec2f& aspect, bool arb, ofVec2f tSize = ofVec2f(1000, 1000), ofVec2f center = ofVec2f(0.5, 0.5), float angle = 1.57f, float scale = 1.0f);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        float getAngle(){ return angle; }
        void setAgnle(float v) { angle = v; }
        
        float getScale() { return scale; }
        void setScale(float v) { scale = v; }
        
        ofVec2f getSize() { return tSize; }
        void setSize(ofVec2f v) { tSize = v; }
        
        ofVec2f getCenter() { return center; }
        void setCenter(ofVec2f v) { center = v; }
        
    private:
        
        ofShader shader;
        ofVec2f tSize, center;
        float angle, scale;
        
    };
}

#endif /* DotScreenPass_hpp */
