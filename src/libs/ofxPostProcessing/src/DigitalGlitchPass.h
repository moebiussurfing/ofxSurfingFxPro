//
//  DigitalGlitchPass.h
//  example_AV_Template
//
//  Created by alp tugan on 5.07.2020.
//

#ifndef DigitalGlitchPass_h
#define DigitalGlitchPass_h

//#include "ofMain.h"
#include "RenderPass.h"
//#include "ofShader.h"

/*
 source: https://github.com/mrdoob/three.js/blob/master/examples/js/shaders/DigitalGlitch.js
 
 uniforms: {
 
 "tDiffuse": { value: null }, //diffuse texture
 "tDisp": { value: null }, //displacement texture for digital glitch squares
 "byp": { value: 0 }, //apply the glitch ?
 "amount": { value: 0.08 },
 "angle": { value: 0.02 },
 "seed": { value: 0.02 },
 "seed_x": { value: 0.02 }, //-1,1
 "seed_y": { value: 0.02 }, //-1,1
 "distortion_x": { value: 0.5 },
 "distortion_y": { value: 0.6 },
 "col_s": { value: 0.05 }
 },
 
 */

namespace itg
{
    class DigitalGlitchPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<DigitalGlitchPass> Ptr;
        
        DigitalGlitchPass(const ofVec2f& aspect, bool arb, float amount = 0.08, float angle = 0.02, float seed = 0.02, float seed_x = 0.02, float seed_y = 0.02, float distortion_x = 0.5, float distortion_y = 0.6, float col_s = 0.05, int byp = 0)  :
        amount(amount), angle(angle), seed(seed), seed_x(seed_x), seed_y(seed_y), distortion_x(distortion_x), distortion_y(distortion_y), col_s(col_s), byp(byp), RenderPass(aspect, arb, "GLITCH DIGITAL") {
            
            
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
            uniform int byp; //should we apply the glitch ?
            
            uniform sampler2D tDiffuse;
            uniform sampler2D tDisp;
            
            uniform float amount;
            uniform float angle;
            uniform float seed;
            uniform float seed_x;
            uniform float seed_y;
            uniform float distortion_x;
            uniform float distortion_y;
            uniform float col_s;
            uniform float time;
            varying vec2 vUv;
            
            
            float rand(vec2 co){
                return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
            }
            
            void main() {
                if(byp<1) {
                    vec2 p = vUv;
                    float xs = floor(gl_FragCoord.x / 0.5);
                    float ys = floor(gl_FragCoord.y / 0.5);
                    //based on staffantans glitch shader for unity https://github.com/staffantan/unityglitch
                    vec4 normal = texture2D (tDisp, p*seed*seed);
                    if(p.y<distortion_x+col_s && p.y>distortion_x-col_s*seed) {
                        if(seed_x>0.){
                            p.y = 1. - (p.y + distortion_y);
                        }
                        else {
                            p.y = distortion_y;
                        }
                    }
                    if(p.x<distortion_y+col_s && p.x>distortion_y-col_s*seed) {
                        if(seed_y>0.){
                            p.x=distortion_x;
                        }
                        else {
                            p.x = 1. - (p.x + distortion_x);
                        }
                    }
                    p.x+=normal.x*seed_x*(seed/5.);
                    p.y+=normal.y*seed_y*(seed/5.);
                    //base from RGB shift shader
                    vec2 offset = amount * vec2( cos(angle), sin(angle));
                    vec4 cr = texture2D(tDiffuse, p + offset);
                    vec4 cga = texture2D(tDiffuse, p);
                    vec4 cb = texture2D(tDiffuse, p - offset);
                    gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
                    //add noise
                    vec4 snow = 100.*amount*vec4(rand(vec2(xs * seed,ys * seed*50.))*0.2);
                    gl_FragColor = gl_FragColor+ snow;
                }
                else {
                    gl_FragColor=texture2D (tDiffuse, vUv);
                }
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
            //shader.setUniformTexture("tDisp", depthTex, 1);
            shader.setUniform1f("time", ofGetElapsedTimef());
            shader.setUniform1f("amount", amount);
            shader.setUniform1f("angle", angle);
            shader.setUniform1f("seed", seed);
            shader.setUniform1f("seed_x", seed_x);
            shader.setUniform1f("seed_y", seed_y);
            shader.setUniform1f("distortion_x", distortion_x);
            shader.setUniform1f("distortion_y", distortion_y);
            shader.setUniform1f("col_s", col_s);
            shader.setUniform1i("byp", byp);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        float getAmount(){ return amount; }
        void setAmount(float v) { amount = v; }
        
        float getAngle(){ return angle; }
        void setAngle(float v) { angle = v; }
        
        float getSeed(){ return seed; }
        void setSeed(float v) { seed = v; }
        
        float getSeedX(){ return seed_x; }
        void setSeedX(float v) { seed_x = v; }
        
        float getSeedY(){ return seed_y; }
        void setSeedY(float v) { seed_y = v; }
        
        float getDistortionX(){ return distortion_x; }
        void setDistortionX(float v) { distortion_x = v; }
        
        float getDistortionY(){ return distortion_y; }
        void setDistortionY(float v) { distortion_y = v; }
        
        float getCols(){ return col_s; }
        void setCols(float v) { col_s = v; }
        
        int getByp(){ return byp; }
        void setByp(int v) { byp = v; }
        

        
    private:
        
        ofShader shader;
        float amount, angle, seed, seed_x, seed_y,distortion_x,distortion_y,col_s;
        int byp;
        
    };
}


#endif /* DigitalGlitchPass_h */
