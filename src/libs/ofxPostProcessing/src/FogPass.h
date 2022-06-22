//
//  FogPass.h
//  RAW_Dystopia-Down_to_earth
//
//  Created by alp tugan on 13.05.2021.
//

#ifndef FogPass_h
#define FogPass_h

#include "RenderPass.h"

namespace itg
{
    class FogPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<FogPass> Ptr;
        
        FogPass(const ofVec2f& aspect, bool arb, glm::vec4 fogColor = glm::vec4(0,0,0,1), float fogStart = 0, float fogEnd = 10):fogColor(fogColor), fogStart(fogStart), fogEnd(fogEnd), RenderPass(aspect, arb, "FOG FILTER") {
            
            // ofTexture & texture1, ofTexture & texture2,
            
            shader.load("shader");
        }
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex) {
            writeFbo.begin();
            
            
            shader.begin();
            
            
            shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
            //shader.setUniformTexture("tDisp", depthTex, 1);
            //shader.setUniform1f("time", ofGetElapsedTimef());
            shader.setUniform1f("fogStart",fogStart );
            shader.setUniform1f("fogEnd", fogEnd);
            shader.setUniform4f("fogColor", fogColor);

            
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setColor(glm::vec4 v) { fogColor = v; }
        void setStart(float v) { fogStart = v; }
        void setEnd(float v) { fogEnd = v; }
        
    private:
        
        ofShader shader;
        glm::vec4 fogColor;
        float fogStart, fogEnd;
        
    };
}

#endif /* FogPass_h */
