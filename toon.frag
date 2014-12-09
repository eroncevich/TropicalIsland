//  Cartoon fragment shader
//  Adapted from Lighthouse3D

varying vec3 N;
varying vec3 L;

uniform sampler2D tex;

//  Shadow Fragment shader

varying vec3 View;
varying vec4 Ambient;
uniform sampler2DShadow depth;

vec4 phong()
{
   //  Emission and ambient color
   vec4 color = Ambient;

   //  Do lighting if not in the shadow
   if (shadow2DProj(depth,gl_TexCoord[1]).a==1.0)
   {
      //  N is the object normal
      vec3 Normal0 = normalize(N);
      //  L is the light vector
      vec3 Light0 = normalize(L);
      vec4 vec =  gl_FrontLightProduct[0].ambient;

      //  Diffuse light is cosine of light and normal vectors
      float Id = dot(Light0,Normal0);
      if (Id>0.0)
      {
         //  Add diffuse
         color += vec;//Id*gl_FrontLightProduct[0].diffuse;
         //  R is the reflected light vector R = 2(Light0.N)Normal0 - Light0
         vec3 R = reflect(-Light0,Normal0);
         //  V is the view vector (eye vector)
         vec3 V = normalize(View);
         //  Specular is cosine of reflected and view vectors
         float Is = dot(R,V);
         if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
      }
   }
   return color;
}


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

   //gl_FragColor = phong() * colorTex;//texture2D(colortex,gl_TexCoord[0].st);

}
