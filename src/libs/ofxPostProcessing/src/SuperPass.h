//
//  SuperPass.h
//  example_AV_Template
//
//  Created by alp tugan on 11.07.2020.
//

/*
 SuperShader combines common simple color manipulation
 - Glow
 - Vignette
 - Brightness / Contrast
 - Hue / Saturation
 
 @author felixturner / http://airtight.cc/
 
 */

#ifndef SuperPass_h
#define SuperPass_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class SuperPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<SuperPass> Ptr;

        SuperPass(const ofVec2f& aspect, bool arb, float glowAmount = 0.5f, float glowSize = 4.0f, float vigOffset = 1, float vigDarkness = 1, float brightness = 0, float contrast = 0, float saturation = 0, float rgbShiftAmount = 0.01):glowAmount(glowAmount), glowSize(glowSize), vigOffset(vigOffset), vigDarkness(vigDarkness), brightness(brightness), contrast(contrast), saturation(saturation), rgbShiftAmount(rgbShiftAmount), RenderPass(aspect, arb, "Super Shader") {
            
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
                                             
                                             uniform float glowSize;
                                             uniform float glowAmount;
                                             uniform vec2 resolution;

                                             uniform float vigOffset;
                                             uniform float vigDarkness;
                                             
                                             uniform float brightness;
                                             uniform float contrast;
                                             
                                             uniform float hue;
                                             uniform float saturation;
                                             
                                             uniform float rgbShiftAmount;
                                             
                                             varying vec2 vUv;
                                             
                                             const float rgbAngle = 0.1;
                                             
                                             void main() {
                                                 float h = glowSize / resolution.x;
                                                 float v = glowSize / resolution.y;
                                                 
                                                 vec4 sum = vec4( 0.0 );
                                                 
                                                 //H Blur
                                                 sum += texture2D( tDiffuse, vec2( vUv.x - 4.0 * h, vUv.y ) ) * 0.051;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x - 3.0 * h, vUv.y ) ) * 0.0918;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x - 2.0 * h, vUv.y ) ) * 0.12245;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x - 1.0 * h, vUv.y ) ) * 0.1531;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y ) ) * 0.1633;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x + 1.0 * h, vUv.y ) ) * 0.1531;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x + 2.0 * h, vUv.y ) ) * 0.12245;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x + 3.0 * h, vUv.y ) ) * 0.0918;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x + 4.0 * h, vUv.y ) ) * 0.051;
                                                 
                                                 //V Blur
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 4.0 * v ) ) * 0.051;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 3.0 * v ) ) * 0.0918;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 2.0 * v ) ) * 0.12245;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 1.0 * v ) ) * 0.1531;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y ) ) * 0.1633;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 1.0 * v ) ) * 0.1531;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 2.0 * v ) ) * 0.12245;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 3.0 * v ) ) * 0.0918;
                                                 sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 4.0 * v ) ) * 0.051;
                                                 
                                                 //orig color
                                                 vec4 col = texture2D( tDiffuse, vUv );
                                                 
                                                 //rgb shift wat=y from center
                                                 vec2 uv = ( vUv - vec2( 0.5 ) );
                                                 float amt = dot( uv, uv );
                                                 vec2 offset = rgbShiftAmount * vec2( cos(rgbAngle), sin(rgbAngle)) * amt;
                                                 vec4 cr = texture2D(tDiffuse, vUv + offset);
                                                 vec4 cga = texture2D(tDiffuse, vUv);
                                                 vec4 cb = texture2D(tDiffuse, vUv - offset);
                                                 col = vec4(cr.r, cga.g, cb.b, cga.a);
                                                 
                                                 //Add Glow
                                                 col = min(col + sum * glowAmount, 1.0);
                                                 
                                                 //vignette
                                                 vec2 uv2 = uv * vec2( vigOffset );
                                                 col = vec4( mix( col.rgb, vec3( 1.0 - vigDarkness ), dot( uv2, uv2 ) ), col.a );
                                                 
                                                 // //BrightnessContrast
                                                 col.rgb += brightness;
                                                 
                                                 if (contrast > 0.0) {
                                                     col.rgb = (col.rgb - 0.5) / (1.0 - contrast) + 0.5;
                                                 } else {
                                                     col.rgb = (col.rgb - 0.5) * (1.0 + contrast) + 0.5;
                                                 }
                                                 
                                                 // // saturation
                                                 float average = (col.r + col.g + col.b) / 3.0;
                                                 if (saturation > 0.0) {
                                                     col.rgb += (average - col.rgb) * (1.0 - 1.0 / (1.001 - saturation));
                                                 } else {
                                                     col.rgb += (average - col.rgb) * (-saturation);
                                                 }
                                                 
                                                 gl_FragColor = col;
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
            shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
            shader.setUniform1f("glowAmount", glowAmount);
            shader.setUniform1f("glowSize", glowSize);
            shader.setUniform1f("vigOffset", vigOffset);
            shader.setUniform1f("vigDarkness", vigDarkness);
            shader.setUniform1f("brightness", brightness);
            shader.setUniform1f("contrast", contrast);
            shader.setUniform1f("saturation", saturation);
            shader.setUniform1f("rgbShiftAmount", rgbShiftAmount);
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setGlowAmount(float v) { glowAmount = v; }
        void setGlowSize(float v) { glowSize = v; }
        void setVigOffset(float v) { vigOffset = v; }
        void setVigDark (float v) { vigDarkness = v; }
        void setBrightness (float v) { brightness = v; }
        void setContrast (float v) { contrast = v; }
        void setSaturation (float v) { saturation = v; }
        void setRGBShiftAmount (float v) { rgbShiftAmount = v; }
        
        
    private:
        
        ofShader shader;
        float glowAmount, glowSize, vigOffset, vigDarkness, brightness, contrast, saturation, rgbShiftAmount;
        
    };
}

#endif /* SuperPass_h */
