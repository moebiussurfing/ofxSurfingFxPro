//
//  NoisePass.h
//  example_AV_Template
//
//  Created by alp tugan on 11.07.2020.
//

#ifndef NoisePass_h
#define NoisePass_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class NoisePass : public RenderPass
    {
    public:
        
        typedef shared_ptr<NoisePass> Ptr;
        
        NoisePass(const ofVec2f& aspect, bool arb, float speed = 0.08f, float amount = 0.128f):speed(speed), amount(amount), RenderPass(aspect, arb, "NOISE GRAINS") {
            
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
                                             
                                             uniform float amount;
                                             uniform float speed;
                                             uniform float time;
                                             varying vec2 vUv;
                                             
                                             
                                             float random(vec2 n, float offset ){
                                                 return .5 - fract(sin(dot(n.xy + vec2( offset, 0. ), vec2(12.9898, 78.233)))* 43758.5453);
                                             }
                                             
                                             void main() {
                                                 vec3 color = texture2D(tDiffuse, vUv).rgb;
                                                 color += vec3( amount * random( vUv, .0001 * speed * time ) );
                                                 gl_FragColor = vec4(color,1.0);
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
            shader.setUniform1f("speed", speed);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setSpeed(float v) { speed = v; }
        void setAmount(float v) { amount = v; }

        
    private:
        
        ofShader shader;
        float speed, amount;
        
    };
}

#endif /* NoisePass_h */
