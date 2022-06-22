//
//  SlantShiftPass.h
//  Shader_AV
//
//  Created by alp tugan on 12.10.2020.
//  Source: https://github.com/loveandsheep/ofxKsmrFragmentFx/blob/master/src/FragFXUnits_SlantShift.h
#ifndef SlantShiftPass_h
#define SlantShiftPass_h

#include "RenderPass.h"
//#include "ofShader.h"

namespace itg
{
    class SlantShiftPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<SlantShiftPass> Ptr;
        
        SlantShiftPass(const ofVec2f& aspect, bool arb,float Volume = 1.0):Volume(Volume), RenderPass(aspect, arb, "SlantShift") {
            
            // ofTexture & texture1, ofTexture & texture2,
            string vertShaderSrc = STRINGIFY(
                                             void main(void)
                                             {
                                                 gl_TexCoord[0] = gl_MultiTexCoord0;
                                                 gl_FrontColor = gl_Color;
                                                 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                                             }
                                             );
            
            string fragShaderSrc = STRINGIFY(
                                             uniform sampler2D tDiffuse;
                                             uniform vec2 resolution;
                                             
                                             uniform float Volume;
                                             uniform float Phase;
                                             
                                             const int   oct  = 8;
                                             const float per  = 0.5;
                                             const float PI   = 3.1415926;
                                             const float cCorners = 1.0 / 16.0;
                                             const float cSides   = 1.0 / 8.0;
                                             const float cCenter  = 1.0 / 4.0;
                                             
                                             // 補間関数
                                             float interpolate(float a, float b, float x){
                                                 float f = (1.0 - cos(x * PI)) * 0.5;
                                                 return a * (1.0 - f) + b * f;
                                             }
                                             
                                             // 乱数生成
                                             float rnd(vec2 p){
                                                 return fract(sin(dot(p ,vec2(12.9898,78.233))) * 43758.5453);
                                             }
                                             
                                             // 補間乱数
                                             float irnd(vec2 p){
                                                 vec2 i = floor(p);
                                                 vec2 f = fract(p);
                                                 vec4 v = vec4(rnd(vec2(i.x,       i.y      )),
                                                               rnd(vec2(i.x + 1.0, i.y      )),
                                                               rnd(vec2(i.x,       i.y + 1.0)),
                                                               rnd(vec2(i.x + 1.0, i.y + 1.0)));
                                                 return interpolate(interpolate(v.x, v.y, f.x), interpolate(v.z, v.w, f.x), f.y);
                                             }
                                             
                                             // ノイズ生成
                                             float noise(vec2 p){
                                                 float t = 0.0;
                                                 for(int i = 0; i < oct; i++){
                                                     float freq = pow(2.0, float(i));
                                                     float amp  = pow(per, float(oct - i));
                                                     t += irnd(vec2(p.x / freq, p.y / freq)) * amp;
                                                 }
                                                 return t;
                                             }
                                             
                                             // シームレスノイズ生成
                                             float snoise(vec2 p, vec2 q, vec2 r){
                                                 return noise(vec2(p.x,       p.y      )) *        q.x  *        q.y  +
                                                 noise(vec2(p.x,       p.y + r.y)) *        q.x  * (1.0 - q.y) +
                                                 noise(vec2(p.x + r.x, p.y      )) * (1.0 - q.x) *        q.y  +
                                                 noise(vec2(p.x + r.x, p.y + r.y)) * (1.0 - q.x) * (1.0 - q.y);
                                             }
                                             
                                             void main(void){
                                                 vec2 st = gl_TexCoord[0].st;
                                                 vec2 t = vec2(st.y / 4.0,Phase * 30.0);
                                                 vec2 s = vec2(st.x / 3.5,Phase * 42.4);
                                                 float ns = (noise(t) - 0.5)*Volume*100.0;
                                                 float nt = (noise(s) - 0.5)*Volume*100.0;
                                                 
                                                 vec3 col;
                                                 
                                                 col.r = texture2D(tDiffuse, vec2(st.x+ns,st.y+nt)).r;
                                                 col.b = texture2D(tDiffuse, vec2(st.x+ns,st.y+nt)).b;
                                                 col.g = texture2D(tDiffuse, vec2(st.x+ns,st.y+nt)).g;
                                                 
                                                 gl_FragColor = vec4(col,1.0);
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
            shader.setUniform1f("Volume", Volume);
            shader.setUniform1f("Phase", ofGetElapsedTimef());
            shader.setUniform2f("resolution",ofGetWidth(), ofGetHeight());
            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            /*glBegin(GL_TRIANGLE_STRIP);
            
            glTexCoord2d(0.0, 0.0);
            glVertex2d(0.0, 0.0);
            
            glTexCoord2d(writeFbo.getWidth(), 0.0);
            glVertex2d(writeFbo.getWidth(), 0.0);
            
            glTexCoord2d(0.0, writeFbo.getHeight());
            glVertex2d(0.0, writeFbo.getHeight());
            
            glTexCoord2d(writeFbo.getWidth(), writeFbo.getHeight());
            glVertex2d(writeFbo.getWidth(), writeFbo.getHeight());
            
            glEnd();*/
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setVolume(float Volume) {this->Volume = Volume;}
        
        
    private:
        
        ofShader shader;
        float Volume;
        
    };
}
#endif /* SlantShiftPass_h */
