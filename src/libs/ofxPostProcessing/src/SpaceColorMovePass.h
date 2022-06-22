//
//  SpaceColorMovePass.h
//  example_AV_Template
//
//  Created by alp tugan on 16.07.2020.
//

#ifndef SpaceColorMovePass_h
#define SpaceColorMovePass_h


#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class SpaceColorMovePass : public RenderPass
    {
    public:
        
        typedef shared_ptr<SpaceColorMovePass> Ptr;
        
        SpaceColorMovePass(const ofVec2f& aspect, bool arb, float opacity = 0.08f):opacity(opacity), RenderPass(aspect, arb, "SPACE COLORS SHADER") {
            
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

                                             const float PI = 3.14159265359;
                                             uniform sampler2D tDiffuse;
                                             uniform float time;
                                             uniform float opacity;
                                             
                                             varying vec2 vUv;
                                             
                                             
                                             
                                             mat2 rotate2d(float _angle){
                                                 return mat2(cos(_angle),-sin(_angle),
                                                             sin(_angle),cos(_angle));
                                             }
                                             
                                             float hash( vec2 p )
                                             {
                                                 float h = dot(p,vec2(127.1,311.7));
                                                 
                                                 return -1.0 + 2.0*fract(sin(h)*43758.5453123);
                                             }
                                             
                                             //cheap value noise https://www.shadertoy.com/view/lsf3WH
                                             float noise( in vec2 p )
                                             {
                                                 vec2 i = floor( p );
                                                 vec2 f = fract( p );
                                                 
                                                 vec2 u = f*f*(3.0-2.0*f);
                                                 
                                                 return mix( mix( hash( i + vec2(0.0,0.0) ),
                                                                 hash( i + vec2(1.0,0.0) ), u.x),
                                                            mix( hash( i + vec2(0.0,1.0) ),
                                                                hash( i + vec2(1.0,1.0) ), u.x), u.y);
                                             }
                                             
                                             void main( )
                                             {
                                                 
                                                 vec2 uv = vUv;
                                                 vec3 orig = texture2D(tDiffuse, uv).rgb;
                                                 
                                                 //rotate uv space
                                                 uv -= 0.5;
                                                 uv = rotate2d( fract(time/40.0)*2.0*PI ) * uv;
                                                 uv += 0.5;
                                                 
                                                 //4 corner grad blue/pink
                                                 vec3 col1 = vec3(uv,1.0);
                                                 
                                                 //4 corner grad pink/orang
                                                 vec3 col2 = vec3(1,uv);
                                                 
                                                 //mix over time
                                                 float t = abs(2.0 * fract(time/10.0) - 1.0);
                                                 vec3 col = mix(col1,col2,t);
                                                 
                                                 //noise clouds additive blend
                                                 //f returns in -1 - 1 range
                                                 //simple 1 octave
                                                 float tn = time/5.0;
                                                 vec2 uvn = uv * 2.; // noise scale,
                                                 float f  = noise( uvn + tn);
                                                 f *= 0.8;
                                                 col += f;
                                                 //darker
                                                 col -= 0.4;
                                                 
                                                 gl_FragColor = vec4(orig + col * opacity, 1.0);
                                                 
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
            shader.setUniform1f("speed", speed);
            shader.setUniform1f("time", ofGetElapsedTimef() * speed);
            shader.setUniform1f("opacity", opacity);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setOpacity(float v) { opacity = v; }
        void setSpeed(float v) { speed = v; } 
    private:
        
        ofShader shader;
        float opacity;
        float speed;
        
    };
}
#endif /* SpaceColorMovePass_h */
