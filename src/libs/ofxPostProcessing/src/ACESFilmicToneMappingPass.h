//
//  ACESFilmicToneMappingPass.h
//  example_AV_Template
//
//  Created by alp tugan on 6.07.2020.
//

#ifndef ACESFilmicToneMappingPass_h
#define ACESFilmicToneMappingPass_h


//#include "ofMain.h"
#include "RenderPass.h"
//#include "ofShader.h"

/*
 source : https://github.com/mrdoob/three.js/blob/master/examples/js/shaders/ACESFilmicToneMappingShader.js
 
 
 */

namespace itg
{
    class ACESFilmicToneMappingPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<ACESFilmicToneMappingPass> Ptr;
        
        ACESFilmicToneMappingPass(const ofVec2f& aspect, bool arb, float exposure = 1.0): exposure(exposure), RenderPass(aspect, arb, "FILTER ACES Film") {
            
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
                                             
                                             uniform float exposure;
                                             uniform sampler2D tDiffuse;
                                             varying vec2 vUv;
                                             
                                             vec3 saturate(vec3 a) {
                                                 return clamp( a, 0.0, 1.0 );
                                             }
                                             // End Ashima 2D Simplex Noise
                                             
                                             vec3 RRTAndODTFit( vec3 v ) {
                                                 
                                                 vec3 a = v * ( v + 0.0245786 ) - 0.000090537;
                                                 vec3 b = v * ( 0.983729 * v + 0.4329510 ) + 0.238081;
                                                 return a / b;
                                                 
                                             }
                                             vec3 ACESFilmicToneMapping( vec3 color ) {
                                                 
                                                 // sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
                                                 const mat3 ACESInputMat = mat3(
                                                                                vec3( 0.59719, 0.07600, 0.02840 ), // transposed from source
                                                                                vec3( 0.35458, 0.90834, 0.13383 ),
                                                                                vec3( 0.04823, 0.01566, 0.83777 )
                                                                                );
                                                 
                                                 // ODT_SAT => XYZ => D60_2_D65 => sRGB
                                                 const mat3 ACESOutputMat = mat3(
                                                                                 vec3(  1.60475, -0.10208, -0.00327 ), // transposed from source
                                                                                 vec3( -0.53108,  1.10813, -0.07276 ),
                                                                                 vec3( -0.07367, -0.00605,  1.07602 )
                                                                                 );
                                                 
                                                 color = ACESInputMat * color;
                                                 
                                                 // Apply RRT and ODT
                                                 color = RRTAndODTFit( color );
                                                 
                                                 color = ACESOutputMat * color;
                                                 
                                                 // Clamp to [0, 1]
                                                 return saturate( color );
                                                 
                                             }
                                             
                                             
                                             
                                             void main() {
                                                 
                                                 vec4 tex = texture2D( tDiffuse, vUv );
                                                 tex.rgb *= exposure / 0.6;
                                                 gl_FragColor = vec4( ACESFilmicToneMapping( tex.rgb ), tex.a );
                                                 
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
            shader.setUniform1f("exposure", exposure);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setExposure(float v) { exposure = v; }

        
    private:
        
        ofShader shader;
        float exposure;
        
    };
}

#endif /* ACESFilmicToneMappingPass_h */
