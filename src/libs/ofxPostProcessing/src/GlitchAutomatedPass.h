//
//  GlitchAutomatedPass.h
//  example_AV_Template
//
//  Created by alp tugan on 13.07.2020.
//

#ifndef GlitchAutomatedPass_h
#define GlitchAutomatedPass_h

//#include "ofMain.h"
#include "RenderPass.h"
//#include "ofShader.h"

/*
 source implemented from : https://www.shadertoy.com/view/MtXBDs
 
 
 */

namespace itg
{
    class GlitchAutomatedPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<GlitchAutomatedPass> Ptr;
        
        GlitchAutomatedPass(const ofVec2f& aspect, bool arb, float speed = 0.6, float amt = 0.2):speed(speed), amt(amt), RenderPass(aspect, arb, "GLITCH AUTOMATED") {
            
            // ofTexture & texture1, ofTexture & texture2,
            
            string vertShaderSrc = STRINGIFY(
                                             varying vec2 vUv;
                                             void main()
                                             {
                                                 //gl_Position = ftransform();
                                                 gl_TexCoord[0] = gl_MultiTexCoord0;
                                                 vUv = gl_TexCoord[0].st;
                                                 // The following one also works
                                                 gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

                                             }
                                             );
            
            string fragShaderSrc = STRINGIFY(
                                             uniform sampler2D tDiffuse;
                                             uniform vec2 iResolution;
                                             uniform float iTime;
                                             uniform float AMT; // 0 - 1
                                             uniform float SPEED; // 0 - 1
                                             varying vec2 vUv;
                                             
                                             //2D (returns 0 - 1)
                                             float random2d(vec2 n) {
                                                 return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
                                             }
                                             
                                             float randomRange (in vec2 seed, in float min, in float max) {
                                                 return min + random2d(seed) * (max - min);
                                             }
                                             
                                             // return 1 if v inside 1d range
                                             float insideRange(float v, float bottom, float top) {
                                                 return step(bottom, v) - step(top, v);
                                             }
                                             
                                             
                                             void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
                                                
                                                float time = floor(iTime * SPEED * 60.0);
                                                vec2 uv = vUv;
                                                
                                                //copy orig
                                                vec3 outCol = texture2D(tDiffuse, uv).rgb;
                                                
                                                //randomly offset slices horizontally
                                                float maxOffset = 0.0;
                                                for (float i = 0.0; i < 4.0 * AMT; i += 1.0) {
                                                    float sliceY = random2d(vec2(time , 2345.0 + float(i))) * 0.25;
                                                    float sliceH = random2d(vec2(time , 9035.0 + float(i))) * 0.5;
                                                    float hOffset = randomRange(vec2(time , 9625.0 + float(i)), -maxOffset, maxOffset);
                                                    vec2 uvOff = uv;
                                                    uvOff.x += hOffset;
                                                    if (insideRange(uv.y, sliceY, fract(sliceY+sliceH)) == 1.0 ){
                                                        outCol = texture2D(tDiffuse, uvOff).rgb;
                                                    }
                                                }
                                                
                                                //do slight offset on one entire channel
                                                float maxColOffset = AMT/6.0;
                                                float rnd = random2d(vec2(time , 9545.0));
                                                vec2 colOffset = vec2(randomRange(vec2(time, 9545.0), -maxColOffset, maxColOffset), randomRange(vec2(time, 7205.0),-maxColOffset,maxColOffset));
                                                if (rnd < 0.33){
                                                    outCol.r = texture2D(tDiffuse, uv + colOffset).r;
                                                    
                                                }else if (rnd < 0.66){
                                                    outCol.g = texture2D(tDiffuse, uv + colOffset).g;
                                                    
                                                } else{
                                                    outCol.b = texture2D(tDiffuse, uv + colOffset).b;
                                                }
                                                
                                                fragColor = vec4(outCol,1.0);
                                            }
                                             
                                             void main() {
                                                 
                                                 mainImage(gl_FragColor, gl_FragCoord.xy);
                                                 
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
            shader.setUniform2f("iResolution", ofGetWidth(), ofGetHeight());
            shader.setUniform1f("iTime", ofGetElapsedTimef());
            shader.setUniform1f("SPEED", speed);
            shader.setUniform1f("AMT", amt);
            
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS

        void setSpeed(float v) { speed = v; }
        void setAmount(float v) {amt = v; }
        
    private:
        
        ofShader shader;
        float amt, speed;
        
    };
}
#endif /* GlitchAutomatedPass_h */
