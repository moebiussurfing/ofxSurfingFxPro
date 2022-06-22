//
//  TiltShiftPass.h
//  example_AV_Template
//
//  Created by alp tugan on 11.07.2020.
//

/**
 
 Custom TiltShift
 
 Added H blur
 Added amount + brightness uniforms
 
 @author felixturner / airtight.cc
 @author alteredq / http://alteredqualia.com/
 
 */

#ifndef TiltShiftPass_h
#define TiltShiftPass_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class TiltShiftPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<TiltShiftPass> Ptr;
        
        TiltShiftPass(const ofVec2f& aspect, bool arb, float focusPos = 0.35f, float range = 0.5f, float offset = 0.05f, float strength = 0.5f):focusPos(focusPos), range(range), offset(offset), strength(strength), RenderPass(aspect, arb, "Tilt Shift") {
            
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
                                             
                                             uniform float focusPos;
                                             uniform float offset;
                                             uniform float range;
                                             uniform float strength;
                                             varying vec2 vUv;
                                             
                                             void main() {
                                                 vec4 orig = texture2D(tDiffuse, vUv);
                                                 vec2 uv = vUv;
                                                 float tiltAmt = 1.0 - smoothstep(focusPos - range,focusPos,uv.y) + smoothstep(focusPos,focusPos + range,uv.y) ;
                                                 
                                                 vec4 blurImg = vec4( 0.0 );
                                                 float offset9 = offset * 0.9;
                                                 float offset7 = offset * 0.7;
                                                 float offset4 = offset * 0.4;
                                                 
                                                 blurImg += texture2D( tDiffuse, vUv.xy );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,   0.4  )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.15,  0.37 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29,  0.29 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.37,  0.15 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.40,  0.0  )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.37, -0.15 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29, -0.29 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.15, -0.37 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,  -0.4  )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.15,  0.37 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29,  0.29 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.37,  0.15 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.4,   0.0  )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.37, -0.15 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29, -0.29 )  * offset );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.15, -0.37 )  * offset );
                                                 
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.15,  0.37 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.37,  0.15 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.37, -0.15 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.15, -0.37 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.15,  0.37 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.37,  0.15 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.37, -0.15 )  * offset9 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.15, -0.37 )  * offset9 );
                                                 
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29,  0.29 )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.40,  0.0  )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29, -0.29 )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,  -0.4  )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29,  0.29 )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.4,   0.0  )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29, -0.29 )  * offset7 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,   0.4  )  * offset7 );
                                                 
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29,  0.29 )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.4,   0.0  )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.29, -0.29 )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,  -0.4  )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29,  0.29 )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.4,   0.0  )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2( -0.29, -0.29 )  * offset4 );
                                                 blurImg += texture2D( tDiffuse, vUv.xy + vec2(  0.0,   0.4  )  * offset4 );
                                                 
                                                 blurImg = blurImg / 41.0;
                                                 
                                                 gl_FragColor = mix(orig, blurImg, tiltAmt * strength);
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
            shader.setUniform1f("focusPos", focusPos);
            shader.setUniform1f("offset", offset);
            shader.setUniform1f("range", range);
            shader.setUniform1f("strength", strength);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setFocusPos(float v) { focusPos = v; }
        void setOffset(float v) { offset = v; }
        void setRange(float v) { range = v; }
        void setStrength(float v) { strength = v; }
        
        
    private:
        
        ofShader shader;
        float focusPos, offset, range, strength;
        
    };
}

#endif /* TiltShiftPass_h */
