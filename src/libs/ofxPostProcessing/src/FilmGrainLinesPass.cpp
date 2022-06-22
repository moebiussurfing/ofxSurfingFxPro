//
//  FilmGrainLines.cpp
//  example_AV_Template
//
//  Created by alp tugan on 4.07.2020.
//

#include "FilmGrainLinesPass.hpp"
//#include "ofMain.h"

namespace itg
{
    FilmGrainLinesPass::FilmGrainLinesPass(const ofVec2f& aspect, bool arb, float time, float nIntensity, float sIntensity, float sCount, float grayscale) :
    time(time), nIntensity(nIntensity), sIntensity(sIntensity), sCount(sCount), grayscale(grayscale), RenderPass(aspect, arb, "FILM GRAIN")
    {
        string vertShaderSrc = STRINGIFY(
        
                                         attribute vec3 position;
                                         varying vec2 vUv;
                                         
                                         void main()
                                         {
                                             gl_TexCoord[0] = gl_MultiTexCoord0;
                                             vUv = gl_TexCoord[0].st;
                                             //gl_Position = gl_ModelViewProjectionMatrix * vec4( position, 1.0 );
                                             
                                             // The following one also works
                                             gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
                                             
                                             //"vUv = uv;",
                                             //"gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );",
                                         }
                                         );
        
        string fragShaderSrc = STRINGIFY(
                                         // control parameter
                                         uniform float time;
                                         uniform float grayscale;
                                          // noise effect intensity value (0 = no effect, 1 = full effect)
                                          uniform float nIntensity;
                                          
                                          // scanlines effect intensity value (0 = no effect, 1 = full effect)
                                          uniform float sIntensity;
                                          
                                          // scanlines effect count value (0 = no effect, 4096 = full effect)
                                          uniform float sCount;
                                          
                                          uniform sampler2D tDiffuse;
                                          
                                          varying vec2 vUv;
                                          
                                          void main() {
                                              // sample the source
                                              vec4 cTextureScreen = texture2D( tDiffuse, vUv );
                                              
                                              // make some noise
                                              float x = vUv.x * vUv.y * time *  1000.0;
                                              x = mod( x, 13.0 ) * mod( x, 123.0 );
                                              float dx = mod( x, 0.01 );
                                              
                                              // add noise
                                              vec3 cResult = cTextureScreen.rgb + cTextureScreen.rgb * clamp( 0.1 + dx * 100.0, 0.0, 1.0 );
                                              
                                              // get us a sine and cosine
                                              vec2 sc = vec2( sin( vUv.y * sCount ), cos( vUv.y * sCount ) );
                                              
                                              // add scanlines
                                              cResult += cTextureScreen.rgb * vec3( sc.x, sc.y, sc.x ) * sIntensity;
                                              
                                              // interpolate between source and result by intensity
                                              cResult = cTextureScreen.rgb + clamp( nIntensity, 0.0,1.0 ) * ( cResult - cTextureScreen.rgb );
                                              
                                              // convert to grayscale if desired
                                              if( grayscale == 1.0) {
                                                  cResult = vec3( cResult.r * 0.3 + cResult.g * 0.59 + cResult.b * 0.11 );
                                              }
                                              
                                              gl_FragColor = vec4( cResult, cTextureScreen.a );
                                          }
                                         );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShaderSrc);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    
    void FilmGrainLinesPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        
        shader.begin();
        
        
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("nIntensity", nIntensity);
        shader.setUniform1f("sIntensity", sIntensity);
        shader.setUniform1f("sCount", sCount);
        shader.setUniform1f("grayscale", grayscale);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
