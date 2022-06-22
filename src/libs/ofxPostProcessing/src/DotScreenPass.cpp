//
//  DotScreenPass.cpp
//  example_AV_Template
//
//  Created by alp tugan on 5.07.2020.
//

#include "DotScreenPass.hpp"
//#include "ofMain.h"

namespace itg
{
    DotScreenPass::DotScreenPass(const ofVec2f& aspect, bool arb, ofVec2f tSize, ofVec2f center, float angle, float scale) :
    tSize(tSize), center(center), angle(angle), scale(scale), RenderPass(aspect, arb, "DOT SCREEN")
    {
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
                                         uniform vec2 center;
                                         uniform float angle;
                                         
                                         uniform float scale;
                                         uniform vec2 tSize;
                                         uniform sampler2D tDiffuse;
                                         varying vec2 vUv;
                                         float pattern() {
                                             float s = sin( angle );
                                             float c = cos( angle );
                                             vec2 tex = vUv * tSize - center;
                                             vec2 point = vec2( c * tex.x - s * tex.y, s * tex.x + c * tex.y ) * scale;
                                             
                                             return ( sin( point.x ) * sin( point.y ) ) * 4.0;
                                         }
                                         
                                         void main() {
                                             vec4 color = texture2D( tDiffuse, vUv );
                                             float average = ( color.r + color.g + color.b ) / 3.0;
                                             gl_FragColor = vec4( vec3( average * 10.0 - 5.0 + pattern() ), color.a );
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShaderSrc);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    
    void DotScreenPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        
        shader.begin();
        
        
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("angle", angle);
        shader.setUniform1f("scale", scale);
        shader.setUniform2f("tSize", tSize);
        shader.setUniform2f("center", center);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
