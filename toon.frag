//  Cartoon fragment shader
//  Adapted from Lighthouse3D

varying vec3 N;
varying vec3 L;
//attribute vec4 gl_MultiTexCoord0;

uniform sampler2D tex;

void main()
{
   float intensity = dot(normalize(N),normalize(L));

   float f;
   if (intensity > 0.3)
      f =0.75;
   else
      f = 0.4;
    vec4 colorTex = texture2D(tex,gl_TexCoord[0].st);

   //gl_FragColor = vec4(gl_Color[0]*f,gl_Color[1]*f,gl_Color[2]*f,1.0);
   gl_FragColor = vec4(colorTex[0]*f+gl_Color[0]*f,colorTex[1]*f+gl_Color[1]*f,
   colorTex[2]*f+gl_Color[2]*f,1.0);
   //gl_FragColor = vec4(colorTex[0]*f+gl_Color[0]*f,colorTex[1]*f,
   //colorTex[2]*f,1.0);

}
