//
//  FirePass.h
//  example_AV_Template
//
//  Created by alp tugan on 5.07.2020.
//

#ifndef FirePass_h
#define FirePass_h

/*
 source : https://threejs.org/examples/?q=shader#webgl_shader_lava
 
 
 */

namespace itg
{
    class FirePass : public RenderPass
    {
    public:
        
        typedef shared_ptr<FirePass> Ptr;
        
        FirePass(const ofVec2f& aspect, bool arb, float fogDensity = 0.45, ofVec3f fogColor = ofVec3f(0.0),  ofVec2f uvScale = ofVec2f(3.0, 1.0)):fogDensity(fogDensity), fogColor(fogColor), uvScale(uvScale), RenderPass(aspect, arb, "Lava Fire") {
            
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
                                             uniform int byp; //should we apply the glitch ?
                                             
                                             uniform sampler2D tDiffuse;
                                             uniform sampler2D tDisp;
                                             
                                             uniform float amount;
                                             uniform float angle;
                                             uniform float seed;
                                             uniform float seed_x;
                                             uniform float seed_y;
                                             uniform float distortion_x;
                                             uniform float distortion_y;
                                             uniform float col_s;
                                             uniform float time;
                                             varying vec2 vUv;
                                             
                                             
                                             float rand(vec2 co){
                                                 return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                                             }
                                             
                                             void main() {
                                                 if(byp<1) {
                                                     vec2 p = vUv;
                                                     float xs = floor(gl_FragCoord.x / 0.5);
                                                     float ys = floor(gl_FragCoord.y / 0.5);
                                                     //based on staffantans glitch shader for unity https://github.com/staffantan/unityglitch
                                                     vec4 normal = texture2D (tDisp, p*seed*seed);
                                                     if(p.y<distortion_x+col_s && p.y>distortion_x-col_s*seed) {
                                                         if(seed_x>0.){
                                                             p.y = 1. - (p.y + distortion_y);
                                                         }
                                                         else {
                                                             p.y = distortion_y;
                                                         }
                                                     }
                                                     if(p.x<distortion_y+col_s && p.x>distortion_y-col_s*seed) {
                                                         if(seed_y>0.){
                                                             p.x=distortion_x;
                                                         }
                                                         else {
                                                             p.x = 1. - (p.x + distortion_x);
                                                         }
                                                     }
                                                     p.x+=normal.x*seed_x*(seed/5.);
                                                     p.y+=normal.y*seed_y*(seed/5.);
                                                     //base from RGB shift shader
                                                     vec2 offset = amount * vec2( cos(angle), sin(angle));
                                                     vec4 cr = texture2D(tDiffuse, p + offset);
                                                     vec4 cga = texture2D(tDiffuse, p);
                                                     vec4 cb = texture2D(tDiffuse, p - offset);
                                                     gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
                                                     //add noise
                                                     vec4 snow = 100.*amount*vec4(rand(vec2(xs * seed,ys * seed*50.))*0.2);
                                                     gl_FragColor = gl_FragColor+ snow;
                                                 }
                                                 else {
                                                     gl_FragColor=texture2D (tDiffuse, vUv);
                                                 }
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

            
            texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
            
            shader.end();
            writeFbo.end();
        }
        
        
        // SETTERS & GETTERS
        void setFogDensity(float v) { fogDensity = v; }
        void setFogColor(ofVec3f v) { fogColor = v; }
        void setUVScale(ofVec2f v) { uvScale = v; }
        
        void setTexture1(ofTexture & v) {texture1 = v;};
        void setTexture2(ofTexture & v) {texture2 = v;};
        
    private:
        
        ofShader shader;
        float fogDensity;
        ofVec3f fogColor;
        ofVec2f uvScale;
        ofTexture texture1, texture2;
        
    };
}


#endif /* FirePass_h */
