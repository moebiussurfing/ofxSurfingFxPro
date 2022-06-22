//
//  ColorInvert.h
//  Shader_AV
//
//  Created by alp tugan on 11.10.2020.
//

#ifndef ColorInvert_h
#define ColorInvert_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class ColorInvert : public RenderPass
    {
    public:
        
        typedef shared_ptr<ColorInvert> Ptr;
        
        ColorInvert(const ofVec2f& aspect, bool arb,
                    float Volume = 1.0):Volume(Volume), RenderPass(aspect, arb, "Strobber") {
            
            // ofTexture & texture1, ofTexture & texture2,
            
            string vertShaderSrc = STRINGIFY(
                                             varying vec2 vUv;
                                             
                                             void main()
                                             {
                                                 gl_TexCoord[0] = gl_MultiTexCoord0;
                                                 vUv = gl_TexCoord[0].st;
                                                 // The following one also works
                                                 gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
                                             }
                                             );
            
            string fragShaderSrc = STRINGIFY(
                                             uniform sampler2D tDiffuse;
                                             varying vec2 vUv;
                                             
                                             uniform float Volume;
                                             uniform float Phase;
                                             //uniform sampler2DRect tex0;
                                             
                                             void main(void){
                                                 vec2 st = gl_TexCoord[0].st;
                                                 vec3 col;
                                                 
                                                 //col.r = texture2DRect(tex0, vec2(st.x,st.y)).r;
                                                 //col.b = texture2DRect(tex0, vec2(st.x,st.y)).b;
                                                 //col.g = texture2DRect(tex0, vec2(st.x,st.y)).g;
                                                 
                                                 col.r = texture2D(tDiffuse, vec2(st.x,st.y)).r;
                                                 col.b = texture2D(tDiffuse, vec2(st.x,st.y)).b;
                                                 col.g = texture2D(tDiffuse, vec2(st.x,st.y)).g;
                                                 
                                                 col.r += ((1.0 - col.r) - col.r) * Volume;
                                                 col.g += ((1.0 - col.g) - col.g) * Volume;
                                                 col.b += ((1.0 - col.b) - col.b) * Volume;
                                                 
                                                 gl_FragColor = vec4(col,1.0);
                                             }
                                             );
            
            shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShaderSrc);
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
            shader.linkProgram();
        }
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex) {
            writeFbo.begin();
            
            
            shader.begin();
            
            
            shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
            shader.setUniform1f("Volume", Volume);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setVolume(float Volume) {this->Volume = Volume;}
        
        
    private:
        
        ofShader shader;
        float Volume;
        
    };
}

#endif /* ColorInvert_h */
