/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a sphere and a cube.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include "math.h"
//#include <OPENAL/al.h>
//#include <OPENAL/alc.h>

int axes=1;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=-90;         //  Azimuth of view angle
int ph=-30;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
double t=0;
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  5;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
double zh2    =  30;  // Light azimuth
float ylight  =   2;  // Elevation of light

int globalLight =1;

//Position values
double vSize =1;

double camX=0;
double camY=71;
double camZ=71;

double speed = .5;

double lineWidth =2;
int sunPos =0;

int celShader=1;

//button presses
int keyStates[256];
int keySpecialStates[256];

void keyOperations ();


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
static void cylVertex(double th,double r,double y){
  double x = r*Cos(th);
  double z = r*Sin(th);
  //  For a sphere at the origin, the position
  //  and normal vectors are the same
  glVertex3d(x,y,z);
}

static void beach(){
  glPushMatrix();
  glRotatef(15,1,0,0);
  glScalef(5,5,5);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,1);

  double theta;
  int period;

  for(theta=360;theta>0;theta-=4){
    glBegin(GL_QUAD_STRIP);
    for(period=0;period<=90;period+=1){
      glTexCoord2f((abs(180-theta))/180.0,period/90.0);
      glNormal3f(-Cos(theta)*Cos(period*2),1,-Sin(theta)*Cos(period*2));
      cylVertex(theta,5.0+period/4.5, 10*Sin(period*2)-8);
      glTexCoord2f((abs(180-(theta-4)))/180.0,period/90.0);
      glNormal3f(-Cos(theta-4)*Cos(period*2),1,-Sin(theta-4)*Cos(period*2));
      cylVertex(theta-4,5.0+period/4.5,10*Sin(period*2)-8);
    }
    glEnd();
  }
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glRotatef(90,1,0,0);

   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=360;th>=0;th-=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
static void island(double x,double y,double z,double r)
{
   int th,ph;
   //  Save transformation
   glPushMatrix();

   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r/2,r);
   glRotatef(90,1,0,0);
   //  White ball
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,1);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=360;th>=0;th-=2*inc)
      {
         glTexCoord2f(.3*(Sin(th)*Cos(ph))+.5,.3*(Cos(th)*Cos(ph))+.5);
         Vertex(th,ph);
         glTexCoord2f(.3*(Sin(th)*Cos(ph+inc))+.5,.3*(Cos(th)*Cos(ph+inc))+.5);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   glBindTexture(GL_TEXTURE_2D,0);
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
}
static void bird(double x,double y,double z,double dir){
  glPushMatrix();
  glTranslatef(x,y,z);
  glRotatef(dir,0,1,0);
  int phi;
  glColor3f(1,1,1);
  glBegin(GL_QUAD_STRIP);
  for(phi=-90;phi<=450;phi+=60){
    glNormal3f(-Sin(phi),-1,0);
    glVertex3f(phi/36.0-5,Cos(phi),-.5);
    glVertex3f(phi/36.0-5,Cos(phi),.5);
  }
  glEnd();
  glBegin(GL_QUAD_STRIP);
  for(phi=-90;phi<=450;phi+=60){
    glNormal3f(-Sin(phi),1,0);
    glVertex3f(phi/36.0-5,.5*Cos(phi)+.5,.5);
    glVertex3f(phi/36.0-5,.5*Cos(phi)+.5,-.5);
  }
  glEnd();
  glBegin(GL_QUAD_STRIP);
  for(phi=-90;phi<=450;phi+=60){
    glNormal3f(0,0,-1);
    glVertex3f(phi/36.0-5,.5*Cos(phi)+.5,-.5);
    glVertex3f(phi/36.0-5,Cos(phi),-.5);
  }
  glEnd();
  glBegin(GL_QUAD_STRIP);
  for(phi=-90;phi<=450;phi+=60){
    glNormal3f(0,0,1);
    glVertex3f(phi/36.0-5,Cos(phi),.5);
    glVertex3f(phi/36.0-5,.5*Cos(phi)+.5,.5);
  }
  glEnd();
  if(celShader)
    glColor3f(0,0,0);
  else
    glColor3f(1,1,1);
  glPopMatrix();
}

static void palmTree(double x,double y, double z)
{
   glPushMatrix();
   int theta;
   glTranslatef(x,y,z);
   double h,phi;
   int incr = 40;
   glRotatef(10*Sin(zh),1,0,0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,2);
   for(h=0;h<10;h+=.0625){
     glBegin(GL_QUAD_STRIP);
     for (theta=360;theta>=0;theta-=incr)
     {
         glTexCoord2f((abs(180-theta))/180.0,h/10);
         glNormal3f(Cos(theta),0,Sin(theta));
         glVertex3d(0.5*Cos(theta)+Cos(h*9)-1,h,0.5*Sin(theta));
         glTexCoord2f((abs(180-theta))/180.0,(h+.0625)/10);
         glNormal3f(Cos(theta),0,Sin(theta));
         glVertex3d(0.5*Cos(theta)+Cos((h+.0625)*9)-1,h+.0625,0.5*Sin(theta));

     }
     glEnd();
   }
   glBindTexture(GL_TEXTURE_2D,0);
   glDisable(GL_TEXTURE_2D);
   glTranslated(-1,0,0);
   int th;
   glColor3f(0,.5,0);
   for(th=0;th<=360;th+=60){
     glPushMatrix();
     glRotated(th,0,1,0);
     glBegin(GL_QUAD_STRIP);
     for(phi=0;phi<=270;phi+=30){
       glNormal3f(Cos(phi),1,0);
       glVertex3f(phi/54.0,10+3*Sin(phi),-.5);
       glVertex3f(phi/54.0,10+3*Sin(phi),.5);
     }
     glEnd();
     glBegin(GL_QUAD_STRIP);
     for(phi=0;phi<=270;phi+=30){
       glNormal3f(-Cos(phi),1,0);
       glVertex3f(phi/54.0,10+3*Sin(phi),.5);
       glVertex3f(phi/54.0,10+3*Sin(phi),-.5);
     }
     glEnd();
     glPopMatrix();
   }
   if(celShader)
     glColor3f(0,0,0);
   else
     glColor3f(1,1,1);
   glPopMatrix();
}
static void boat(double x, double y, double z){
  glPushMatrix();
  glTranslatef(x,y,z);
  int phi, i;

  glColor3f(.2,.2,.2);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glVertex3f(3,1.05,2);
  glVertex3f(3,1.05,10);
  glVertex3f(2,1.05,2);
  glVertex3f(2,1.05,10);
  glEnd();

  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glVertex3f(-2,1.05,2);
  glVertex3f(-2,1.05,10);
  glVertex3f(-3,1.05,2);
  glVertex3f(-3,1.05,10);
  glEnd();
  if(celShader)
    glColor3f(0,0,0);
  else
    glColor3f(1,1,1);


  glEnable(GL_TEXTURE_2D);
  for(i=0;i<5;i++){
    //glPushMatrix();
    glTranslatef(0,0,2);
    glBindTexture(GL_TEXTURE_2D,2);
    glBegin(GL_QUAD_STRIP);
    for(phi=0;phi<=360;phi+=40){
      glNormal3f(0,Sin(phi),Cos(phi));
      glTexCoord2f((abs(180-phi))/180.0,0);
      glVertex3f(5,Sin(phi),Cos(phi));
      glTexCoord2f((abs(180-phi))/180.0,1);
      glVertex3f(-5,Sin(phi),Cos(phi));
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D,3);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1,0,0);
    glVertex3f(-5,0,0);
    for(phi=360;phi>=0;phi-=40){
      //glTexCoord2f(.3*(Cos(phi))+.5,.3*(Sin(phi))+.5);
      glVertex3f(-5,Sin(phi),Cos(phi));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(1,0,0);
    glVertex3f(5,0,0);
    for(phi=0;phi<=360;phi+=40){
      glVertex3f(5,Sin(phi),Cos(phi));
    }
    glEnd();
    //glPopMatrix();
  }
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
}

static void tornado(double x, double y, double z, double r, double rotation){
   int theta;
   int phi;

   glPushMatrix();
   glTranslated(vSize*x,vSize*y,vSize*z);
   glRotated(rotation,0,1,0);
   glScaled(r,2*r,r);
   glColor3f(.5,.5,.5);
   for(theta=0;theta<1440;theta+=10){
     glBegin(GL_QUAD_STRIP);
     for(phi=0;phi<=360;phi+=40){
       glNormal3f(Cos(theta)*Cos(phi),Sin(phi),Sin(theta)*Cos(phi));
       glVertex3f(theta/72.0*Cos(theta)+Cos(theta)*Cos(phi),theta/72.0+Sin(phi),theta/72.0*Sin(theta)+Sin(theta)*Cos(phi));
       glNormal3f(Cos(theta+10)*Cos(phi),Sin(phi),Sin(theta+10)*Cos(phi));
       glVertex3f((theta+10)/72.0*Cos(theta+10)+Cos(theta+10)*Cos(phi),(theta+10)/72.0+Sin(phi),(theta+10)/72.0*Sin(theta+10)+Sin(theta+10)*Cos(phi));
     }
     glEnd();
   }
   if(celShader)
     glColor3f(0,0,0);
   else
     glColor3f(1,1,1);


   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //int i;
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_CULL_FACE);
   keyOperations();

   glEnable(GL_LINE_SMOOTH);
   //  Undo previous transformations
   glLoadIdentity();
   //keyOperations ();
   //glClearColor(.5,.5,1,0);
   double sunX =abs(sunPos-90)/90.0;
   glClearColor(.5,.1-.1*sunX*sunX*sunX,1-sunX*sunX*sunX,0);
   //  Perspective - set eye position
   if (mode==0)
   {
      gluLookAt(camX,camY,camZ, camX+Cos(th),camY+Sin(ph),camZ+Sin(th) , 0,Cos(ph),0);
   }
   else if (mode==1)
   {
        double radius = sqrt(camX*camX + camZ*camZ +camY*camY);
        camX = radius*Cos(2*zh2)*Cos(ph);
        camZ = radius*Sin(2*zh2)*Cos(ph);
        th = (int)(2*zh2+180)%360;
        if(ph>-5){
          ph=-5;
        }
        //ph = -26.5;
        camY = -radius*Sin(ph);
        //gluLookAt(camX,camY,camZ,10*Sin(ph),10*Cos(th)*Cos(ph), camX,camY,camZ, 0,Cos(ph),0);
        gluLookAt(camX,camY,camZ,0,0,0,0,1,0);
   }

   float Position[]  = {400*Cos(90)*Sin((sunPos)-90),400*Cos((sunPos)-90),400*Sin(90)*Sin(sunPos-90),1.0};

   //  Draw light position as sphere (still no lighting here)
   glColor3f(1,.7,0);
  // float sunColor[]  = {1,.5,0,1.0};
   glDisable(GL_LIGHTING);
   glPushMatrix();
   //glEnable(GL_BLEND);
   //glBlendFunc (GL_CONSTANT_COLOR ,GL_ZERO);
   glTranslated(Position[0],Position[1],Position[2]);
   ball(0,0,0,20);
   //glDisable(GL_BLEND);
   glPopMatrix();
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   float Ambient[]  = {.1,.1,.1,1.0};
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   float Diffuse[]  = {.7,.7,.7,1.0};
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    float Specular[]  = {.4,.4,.4,1.0};
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);





   if(celShader){
     //  Draw polygon outlines
      glPolygonMode(GL_FRONT,GL_LINE);
      glCullFace(GL_BACK);
      glLineWidth(lineWidth);

      //Make outline black
      glEnable (GL_BLEND);
      glEnable (GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(0,100);
      glBlendFunc (GL_ZERO ,GL_ZERO);
      /*Insert All Objects Here*/

      beach();
      palmTree(65,8,0);
      island(0,0,20,20);
      palmTree(60,20,-40);
      palmTree(-20,27,-70);
      palmTree(-45,20, -45);
      palmTree(-75,7,10);
      palmTree(40,22,-50);

      boat(0,2*Sin(zh-15),-20);

      glPushMatrix();
      glTranslated(Position[0],Position[1],Position[2]);
      ball(0,0,0,20);
      glPopMatrix();

      glPushMatrix();
      glTranslated(0,11,20);
      glRotated(10*t,0,1,0);
      glutSolidTeapot(1);
      glPopMatrix();

      bird(100*Cos(zh2),80,100*Sin(zh2), zh2);
      bird(200*Cos(-zh2/2.0),65,200*Sin(-zh2/2.0), zh2/2.0);
      bird(70*Cos(2*zh2),120,70*Sin(2*zh2), -2*zh2);
      bird(150*Cos(-zh2+30),100,150*Sin(-zh2+30), zh2-30);

      tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),1,4*zh);
      tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.5,16*zh+10);
      tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.9,8*zh+30);
      tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.95,6*zh+90);

      tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),1,4*zh);
      tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.5,16*zh+10);
      tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.9,8*zh+30);
      tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.95,6*zh+90);



      glDisable(GL_BLEND);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glCullFace(GL_FRONT);
   }
   if(celShader)
     glColor3f(0,0,0);
   else
     glColor3f(1,1,1);
   glUseProgram(celShader);
   //glEnable(GL_BLEND);
   //glBlendFunc (GL_ONE ,GL_ZERO);
   /*Insert All Objects Here Also*/
   beach();
   palmTree(65,8,0);
   palmTree(60,20,-40);
   palmTree(-20,27,-70);
   palmTree(-45,20, -45);
   palmTree(-75,7,10);
   palmTree(40,22,-50);
   island(0,0,20,20);
   bird(100*Cos(zh2),80,100*Sin(zh2), zh2);
   bird(200*Cos(-zh2/2.0),65,200*Sin(-zh2/2.0), zh2/2.0);
   bird(70*Cos(2*zh2),120,70*Sin(2*zh2), -2*zh2);
   bird(150*Cos(-zh2+30),100,150*Sin(-zh2+30), zh2-30);

   boat(0,2*Sin(zh-15),-20);
   tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),1,4*zh);
   tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.5,16*zh+10);
   tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.9,8*zh+30);
   tornado(100+20*Cos(zh2*4.0),2*Sin(zh),100+20*Sin(zh2*4.0),.95,6*zh+90);

   tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),1,4*zh);
   tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.5,16*zh+10);
   tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.9,8*zh+30);
   tornado(100+20*Cos(zh2*4.0+180),2*Sin(zh),100+20*Sin(zh2*4.0+180),.95,6*zh+90);

   /*Ocean*/
   //glDisable(GL_BLEND);
   glPushMatrix();
   glTranslated(0,11,20);
   glRotated(10*t,0,1,0);
   glColor3f(1,.8,0);
   glutSolidTeapot(1);
   glPopMatrix();

   glColor3f(0,0,.5);
   glBegin(GL_QUAD_STRIP);
   glNormal3f(0,1,0);
   glVertex3d(1000,2*Sin(zh),1000);
   glVertex3d(-1000,2*Sin(zh),1000);
   glVertex3d(1000,2*Sin(zh),-1000);
   glVertex3d(-1000,2*Sin(zh),-1000);
   glEnd();
   glUseProgram(0);

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s \n",
     th,ph,dim,fov,mode?"Perpective":"FP");
   Print("Pos(X,Y,Z)=(%d,%d,%d)",camX,camY,camZ);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   zh2 = fmod(11.25*t,1440.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   if(key==GLUT_KEY_RIGHT||key==GLUT_KEY_LEFT||key==GLUT_KEY_UP||key==GLUT_KEY_DOWN){
     keySpecialStates[key]=1;
   }
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   else if (key== GLUT_KEY_PAGE_DOWN)
      dim+=.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   //Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   //glutPostRedisplay();
}
void upSpecial(int key,int x,int y)
{
  if(key==GLUT_KEY_UP){
    keySpecialStates[GLUT_KEY_UP]=0;
  }else if(key==GLUT_KEY_DOWN){
    keySpecialStates[GLUT_KEY_DOWN]=0;
  }
  else if(key==GLUT_KEY_RIGHT){
    keySpecialStates[GLUT_KEY_RIGHT]=0;
  }else if(key==GLUT_KEY_LEFT){
    keySpecialStates[GLUT_KEY_LEFT]=0;
  }
}

void keyOperations () {
  if(keyStates['w']==1){
    camX+=Cos(th)*speed;
    camZ+=Sin(th)*speed;
    camY+=Sin(ph)*speed;
  }
  if(keyStates['a']==1){
    camX+=Sin(th)*speed;
    camZ+=-Cos(th)*speed;
  }
  if (keyStates['d']==1) {
    camX+=-Sin(th)*speed;
    camZ+=Cos(th)*speed;
  }
  if (keyStates['s']==1) {
    camX-=Cos(th)*speed;
    camZ-=Sin(th)*speed;
    camY-=Sin(ph)*speed;
  }
  if(keySpecialStates[GLUT_KEY_UP]==1){
    ph+=3;
    if(ph>89)
      ph=89;
  }
  if(keySpecialStates[GLUT_KEY_RIGHT]==1){
    th+=3;
  }
  if(keySpecialStates[GLUT_KEY_LEFT]==1){
    th-=3;
  }
  if(keySpecialStates[GLUT_KEY_DOWN]==1){
    ph-=3;
    if(ph<-89)
      ph=-89;
  }
  if (keyStates['q'] ==1) {
    sunPos-=1;
    if(sunPos<0)
      sunPos =0;
  }
  else if (keyStates['e'] ==1) {
    sunPos+=1;
    if(sunPos>180)
      sunPos =180;
  }
}

void upKey(unsigned char ch,int x,int y)
{
  if (ch=='w') {
    keyStates['w'] =0;
  }
  if (ch=='a') {
    keyStates['a'] =0;
  }
  if (ch=='d') {
    keyStates['d'] =0;
  }
  if (ch=='s') {
    keyStates['s'] =0;
  }
  if (ch=='q') {
    keyStates['q'] =0;
  }
  if (ch=='e') {
    keyStates['e'] =0;
  }
}
/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);

   else if (ch=='w') {
     keyStates['w'] =1;
   }
   else if (ch=='a') {
     keyStates['a'] =1;
   }
   else if (ch=='d') {
     keyStates['d'] =1;
   }
   else if (ch=='s') {
     keyStates['s'] =1;
   }
   else if (ch=='q') {
     keyStates['q'] =1;
   }
   else if (ch=='e') {
     keyStates['e'] =1;
   }
   else if (ch=='f') {
     celShader =!celShader;
   }
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   else if (ch == '1')
      globalLight = !globalLight;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if(ch == 'm')
      mode = !mode;
   //  Reproject
   //Project(fov,asp,dim);
   //  Animate if requested
   //glutIdleFunc(idle);//move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   //glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //resize width
   lineWidth = ((float)height)/133.f;
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}


/*
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}
/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   int i;
   for(i=0;i<256;++i){
     keyStates[i]=0;
     keySpecialStates[i]=0;
   }
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(500,500);
   glutCreateWindow("Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutSpecialUpFunc(upSpecial);
   glutKeyboardUpFunc(upKey);
   glutKeyboardFunc(key);

   glutIdleFunc(idle);

   celShader = CreateShaderProg("toon.vert","toon.frag");
   LoadTexBMP("BeachSand3.bmp");
   LoadTexBMP("Bark.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
