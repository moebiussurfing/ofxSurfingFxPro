//
//  DitherPass.h
//  example_AV_Template
//
//  Created by alp tugan on 6.08.2020.
//

#ifndef DitherPass_h
#define DitherPass_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class DitherPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<DitherPass> Ptr;
        
        DitherPass(const ofVec2f& aspect, bool arb, float scale = 1.0f):scale(scale), RenderPass(aspect, arb, "DITHER") {
            
            // ofTexture & texture1, ofTexture & texture2,
            
            string vertShaderSrc = STRINGIFY(
                                             varying vec4 vertTexCoord;
                                             
                                             void main()
                                             {
                                                 gl_TexCoord[0] = gl_MultiTexCoord0;
                                                 vertTexCoord = gl_TexCoord[0];
                                                 // The following one also works
                                                 gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
                                             }
                                             );
            
            string fragShaderSrc = STRINGIFY(
                                             
                                             // Ordered dithering aka Bayer matrix dithering http://devlog-martinsh.blogspot.com.ar/2011/03/glsl-dithering.html
                                             
                                            uniform float scale;
                                            uniform sampler2D tDiffuse;
                                            varying vec4 vertTexCoord;
                                             uniform float time;
                                             
                                             
                                            float find_closest(int x, int y, float c0)
                                            {
                                                vec4 dither[4];
                                                
                                                dither[0] = vec4( 1.0, 33.0, 9.0, 41.0);
                                                dither[1] = vec4(49.0, 17.0, 57.0, 25.0);
                                                dither[2] = vec4(13.0, 45.0, 5.0, 37.0);
                                                dither[3] = vec4(61.0, 29.0, 53.0, 21.0);
                                                
                                                float limit = 0.0;
                                                if(x < 4)
                                                {
                                                    limit = (dither[x][y]+1.0)/64.0;
                                                }
                                                
                                                if(c0 < limit)
                                                {
                                                    return 0.0;
                                                    
                                                }else{
                                                    
                                                    return (0.6 + sin(time*2.0) * 0.4);
                                                }
                                                
                                            }
                                             
                                             
                                            void main(void)
                                            {
                                             vec4 lum = vec4(0.299, 0.587, 0.114, 0.0);
                                             float grayscale = dot( texture2D(tDiffuse, vertTexCoord.xy), lum);
                                             vec3 rgb = texture2D(tDiffuse, vertTexCoord.xy).rgb;
                                                vec3 org = rgb;
                                             
                                             vec2 xy = gl_FragCoord.xy * scale;
                                             int x = int(mod(xy.x, 4.0));
                                             int y = int(mod(xy.y, 4.0));
                                             
                                             vec3 finalRGB;
                                             
                                             finalRGB.r = find_closest(x, y, rgb.r);
                                             finalRGB.g = find_closest(x, y, rgb.g);
                                             finalRGB.b = find_closest(x, y, rgb.b);
                                                
                                             
                                             float final = find_closest(x, y, grayscale);
                                             
                                             gl_FragColor = vec4(org +finalRGB, 1.0);
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
            shader.setUniform1f("scale", scale);
            shader.setUniform1f("time", ofGetElapsedTimef());
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setScale(float v) { scale = v; }
    private:
        
        ofShader shader;
        float scale;
        
    };
}
#endif /* DitherPass_h */
