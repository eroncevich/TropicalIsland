//  Cartoon fragment shader
//  Adapted from Lighthouse3D

varying vec3 N;
varying vec3 L;

uniform sampler2D tex;

//  Shadow Fragment shader

varying vec3 View;
varying vec4 Ambient;
uniform sampler2DShadow depth;


void main()
{
   float intensity = dot(normalize(N),normalize(L));

   float f;
   if (intensity > 0.2)
      f =0.75;
   else
      f = 0.4;
    vec4 colorTex = texture2D(tex,gl_TexCoord[0].st);

   //gl_FragColor = vec4(gl_Color[0]*f,gl_Color[1]*f,gl_Color[2]*f,1.0);
   gl_FragColor = vec4(colorTex[0]*f+gl_Color[0]*f,colorTex[1]*f+gl_Color[1]*f,
   colorTex[2]*f+gl_Color[2]*f,1.0);

   //gl_FragColor = phong() * colorTex;//texture2D(colortex,gl_TexCoord[0].st);

}
