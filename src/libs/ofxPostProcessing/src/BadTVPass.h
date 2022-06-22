//
//  BadTVPass.h
//  example_AV_Template
//
//  Created by alp tugan on 6.07.2020.
//

#ifndef BadTVPass_h
#define BadTVPass_h

//#include "ofMain.h"
#include "RenderPass.h"
//#include "ofShader.h"

/*
 source : https://github.com/felixturner/bad-tv-shader
 
 
 */

namespace itg
{
    class BadTVPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<BadTVPass> Ptr;
        
        BadTVPass(const ofVec2f& aspect, bool arb, float distortion = 3.0, float distortion2 = 5.0, float speed = 0.2, float rollSpeed = 0.1):distortion(distortion), distortion2(distortion2), speed(speed), rollSpeed(rollSpeed), RenderPass(aspect, arb, "GLITCH BAD TV") {
            
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
                                             uniform float time;
                                             uniform float distortion;
                                             uniform float distortion2;
                                             uniform float speed;
                                             uniform float rollSpeed;
                                             varying vec2 vUv;
                                             
                                             
                                             // Start Ashima 2D Simplex Noise
                                             vec3 mod289(vec3 x) {
                                               return x - floor(x * (1.0 / 289.0)) * 289.0;
                                             }
                                             
                                             vec2 mod289(vec2 x) {
                                               return x - floor(x * (1.0 / 289.0)) * 289.0;
                                             }
                                             
                                             vec3 permute(vec3 x) {
                                               return mod289(((x*34.0)+1.0)*x);
                                             }
                                             
                                             float snoise(vec2 v)
                                            {
                                               const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0,
                                                                   0.366025403784439,  // 0.5*(sqrt(3.0)-1.0),
                                                                  -0.577350269189626,  // -1.0 + 2.0 * C.x,
                                                                   0.024390243902439); // 1.0 / 41.0,
                                               vec2 i  = floor(v + dot(v, C.yy) );
                                               vec2 x0 = v -   i + dot(i, C.xx);
                                             
                                               vec2 i1;
                                               i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
                                               vec4 x12 = x0.xyxy + C.xxzz;
                                                x12.xy -= i1;
                                             
                                               i = mod289(i); // Avoid truncation effects in permutation,
                                               
                                                vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));
                                             
                                               vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
                                               m = m*m ;
                                               m = m*m ;
                                             
                                               vec3 x = 2.0 * fract(p * C.www) - 1.0;
                                               vec3 h = abs(x) - 0.5;
                                               vec3 ox = floor(x + 0.5);
                                               vec3 a0 = x - ox;
                                             
                                               m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
                                             
                                               vec3 g;
                                               g.x  = a0.x  * x0.x  + h.x  * x0.y;
                                               g.yz = a0.yz * x12.xz + h.yz * x12.yw;
                                               return 130.0 * dot(m, g);
                                             }
                                             
                                             // End Ashima 2D Simplex Noise
                                             
                                             void main() {
                                             
                                                 vec2 p = vUv;
                                                 float ty = time*speed;
                                                 float yt = p.y - ty;
                                                 //smooth distortion
                                                 float offset = snoise(vec2(yt*3.0,0.0))*0.2;
                                                 // boost distortion
                                                 offset = offset*distortion * offset*distortion * offset;
                                                 //add fine grain distortion
                                                 offset += snoise(vec2(yt*50.0,0.0))*distortion2*0.001;
                                                 //combine distortion on X with roll on Y
                                                 
                                                 gl_FragColor = texture2D(tDiffuse,  vec2(fract(p.x + offset),fract(p.y-time*rollSpeed) ));   
                                             
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
            shader.setUniform1f("distortion", distortion);
            shader.setUniform1f("distortion2", distortion2);
            shader.setUniform1f("speed", speed);
            shader.setUniform1f("rollSpeed", rollSpeed);
            
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setDistortion(float v) { distortion = v; }
        void setDistortion2(float v) { distortion2 = v; }
        void setSpeed(float v) { speed = v; }
        
        void setRollSpeed(float v) {rollSpeed = v;};
        
    private:
        
        ofShader shader;
        float distortion, distortion2, speed, rollSpeed;
        
    };
}

#endif /* BadTVPass_h */
