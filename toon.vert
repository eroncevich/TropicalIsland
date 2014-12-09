//  Cartoon vertex shader
//  Adapted from Lighthouse3D

varying vec3 N;
varying vec3 L;

varying vec3 View;
varying vec4 Ambient;

void main()
{
	 gl_FrontColor = gl_Color;
	 gl_TexCoord[0] = gl_MultiTexCoord0;
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  L is the light vector
   L = vec3(gl_LightSource[0].position) - P;
   //  Normal
   N = gl_NormalMatrix * gl_Normal;

	 View = -P;
	 Ambient = gl_FrontLightProduct[0].ambient;// + gl_LightModel.ambient*gl_FrontMaterial.ambient;

	 vec4 X = gl_ModelViewMatrix*gl_Vertex;
   gl_TexCoord[1].s = dot(gl_EyePlaneS[1],X);
   gl_TexCoord[1].t = dot(gl_EyePlaneT[1],X);
   gl_TexCoord[1].p = dot(gl_EyePlaneR[1],X);
   gl_TexCoord[1].q = dot(gl_EyePlaneQ[1],X);
   gl_TexCoord[1] /= gl_TexCoord[1].q;



   //  Transformed vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
