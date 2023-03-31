////////////////////////////////////////////////////////////////
// Department of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2019-20
//
// Authors: Toby Howard and Arturs Bekasovs
//
/////////////////////////////////////////////////////////////////

#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "bitmap.c"

#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#define MAX_FIRE 200     // # of firesworks
#define MAX_PARTICLES 24 // # sub particles after explosion
#define MAX_TAIL 20      // # trail particle
#define DEG_TO_RAD 0.017453293

#define TOP_VIEW 1
#define ECLIPTIC_VIEW 2
#define SHIP_VIEW 3
#define EARTH_VIEW 4
#define RUN_SPEED  5
#define TURN_ANGLE 4.0

typedef struct {
   float r, g, b;      /* color */
   float x, y, z;      /* position  */
   float xs, ys, zs;   /* speed  */
} Particle;

typedef struct {
   Particle particle[MAX_PARTICLES][MAX_TAIL]; // fireworks array
   float life, fade, rad; // lifetime, fade rate, direction after explosion
   //for space effciency, move this property to struct Fire
   short up;         /* up or down */
} Fire;

GLfloat colors[12][3] = { // Rainbow Of Colors
   {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
   {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
   {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

Fire fire[MAX_FIRE];

// Display list for coordinate axis
GLuint axisList;

GLdouble lat,     lon;              /* View angles (degrees)    */
GLdouble mlat,    mlon;             /* Mouse look offset angles */
GLfloat  eyex,    eyey,    eyez;    /* Eye point                */
GLfloat  centerx, centery, centerz; /* Look point               */

int AXIS_SIZE= 200;
int axisEnabled= 1, t=0, current_view=3, move;
float slowdown = 0.02, zoom = 3;
float gravity = -0.35, wind = -0.05, var_speed = 15, var_life = 0.08;

//trivial vars irrevalant to rendering
int print_count = 0, print_flag = 1, FPS_count = 0, FPS_sum = 0, FPS_init, axis_flag = 0, eff_switch = 0;
FILE *fp1, *fp2;
float fps = 0;
struct timespec t1, t2;

BITMAPINFO *TexInfo; // Texture bitmap information
GLubyte    *TexBits; // Texture bitmap pixel bits
float size = 0.4f;

///////////////////////////////////////////////
void resetFire(int i) {
    // init position
    float xtemp = rand()%(4*MAX_FIRE) - (2*MAX_FIRE);
    float ztemp = rand()%(4*MAX_FIRE) - (2*MAX_FIRE);

    fire[i].life = ((float)(rand()%100))/100 +var_life;//1.0f;
    fire[i].fade = (float) ((rand()%100)/20 + 0.003);
    fire[i].rad  = rand()%3 + 5.0f;
    fire[i].up = 1;
    if(eff_switch == 1){
      Particle* pat = &fire[i].particle[0][0];
      //init color
      pat->r = 1.0f; pat->g = 1.0f; pat->b = 1.0f;
      //emit position
      pat->x = xtemp; pat->y = 0; pat->z = ztemp;
      //init velocity
      pat->xs = 0.0f; pat->ys = rand()%5 + var_speed; pat->zs = 0.0f;
      //init acceleration

      //initialize tail: set position of all tails to have the same positoion with head
      for(int k = 1; k < MAX_TAIL; k++) {
         pat = &fire[i].particle[0][k];
         pat->x = fire[i].particle[0][0].x;
         pat->y = fire[i].particle[0][0].y;
         pat->z = fire[i].particle[0][0].z;
      }
      //for k end
    }
    else{
      float speed = rand()%5 + var_speed;
      for (int j = 0; j < MAX_PARTICLES; j++) {
         Particle* pat = &fire[i].particle[j][0];
         //init color
         pat->r = 1.0f; pat->g = 1.0f; pat->b = 1.0f;
         //emit position
         pat->x = xtemp; pat->y = 0; pat->z = ztemp;
         //init velocity
         pat->xs = 0.0f; pat->ys = speed; pat->zs = 0.0f;
         //init acceleration

         //initialize tail
         for(int k = 1; k < MAX_TAIL; k++) {
            pat = &fire[i].particle[j][k];
            pat->x = fire[i].particle[j][0].x;
            pat->y = fire[i].particle[j][0].y;
            pat->z = fire[i].particle[j][0].z;
         } //for k end
      }//for j end
    }

}

void DrawGLScene()	{ // Here's Where We Do All The Drawing

  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_COLOR,GL_ONE);


   short i,j,k;
   // update particle position and lifetime
   for(i = 0; i < MAX_FIRE; i++) {
     if(eff_switch == 1){
      if(fire[i].up==1){
        for(k = 0; k < MAX_TAIL; k++) {
          float x = fire[i].particle[0][k].x;
          float y = fire[i].particle[0][k].y;
          float z = fire[i].particle[0][k].z;
          float dt = 1 - (float)k/MAX_TAIL;
          glColor4f(fire[i].particle[0][0].r,
            fire[i].particle[0][0].g,
            fire[i].particle[0][0].b,
            fire[i].life*dt);
          glBegin(GL_QUADS);   // Build Quad From A Quadrangle
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(x+size, y+size);
            glVertex3f(x+size,y+size,z); // Top Right
            glTexCoord2f(x-size, y+size);
            glVertex3f(x-size,y+size,z); // Top Left
            glTexCoord2f(x+size, y-size);
            glVertex3f(x+size,y-size,z); // Bottom Right
            glTexCoord2f(x-size, y-size);
            glVertex3f(x-size,y-size,z); // Bottom Left
          glEnd(); // Done Building Triangle Strip
        }
         // update position
        for(k = MAX_TAIL-1; k > 0; k--) {
            fire[i].particle[0][k].x = fire[i].particle[0][k-1].x;
            fire[i].particle[0][k].y = fire[i].particle[0][k-1].y;
            fire[i].particle[0][k].z = fire[i].particle[0][k-1].z;
        } //for k end
        fire[i].particle[0][0].x += fire[i].particle[0][0].xs / 20;
        fire[i].particle[0][0].y += fire[i].particle[0][0].ys / 20;
        fire[i].particle[0][0].z += fire[i].particle[0][0].zs / 20;
        // update y speed
        float yd = gravity;
        fire[i].particle[0][0].ys += yd;
        // update x speed
        float xd = wind;
        fire[i].particle[0][0].xs += xd;
        //when start going down, update all sub-particles in the particle
        if (fire[i].particle[0][0].ys < -yd) {
           int color;
           fire[i].up = 0;

           for(j = 0;j < MAX_PARTICLES; j++){
             fire[i].particle[j][0] = fire[i].particle[0][0];
             color = rand()%12;
             fire[i].particle[j][0].r = colors[color][0];
             fire[i].particle[j][0].g = colors[color][1];
             fire[i].particle[j][0].b = colors[color][2];
             // x-z speed
             double radian = 3.14159*j*15.0/180.0;
             fire[i].particle[j][0].xs = (float)(fire[i].rad*sin(radian)) + xd;
             fire[i].particle[j][0].zs = (float)(fire[i].rad*cos(radian));
           }
        }
      }
      else {
        for (j = 0; j < MAX_PARTICLES; j++) {
          for(k = 0; k < MAX_TAIL; k++) {
            float x = fire[i].particle[j][k].x;
            float y = fire[i].particle[j][k].y;
            float z = fire[i].particle[j][k].z;
            float dt = 1 - (float)k/MAX_TAIL;
            glColor4f(fire[i].particle[j][0].r,
              fire[i].particle[j][0].g,
              fire[i].particle[j][0].b,
              fire[i].life*dt);
            glBegin(GL_QUADS);   // Build Quad From A Quadrangle
              glNormal3f(0.0f, 0.0f, 1.0f);
              glTexCoord2f(x+size, y+size);
              glVertex3f(x+size,y+size,z); // Top Right
              glTexCoord2f(x-size, y+size);
              glVertex3f(x-size,y+size,z); // Top Left
              glTexCoord2f(x+size, y-size);
              glVertex3f(x+size,y-size,z); // Bottom Right
              glTexCoord2f(x-size, y-size);
              glVertex3f(x-size,y-size,z); // Bottom Left
            glEnd(); // Done Building Triangle Strip
          }
          // update position
          for(k = MAX_TAIL-1; k > 0; k--) {
            fire[i].particle[j][k].x = fire[i].particle[j][k-1].x;
            fire[i].particle[j][k].y = fire[i].particle[j][k-1].y;
            fire[i].particle[j][k].z = fire[i].particle[j][k-1].z;
          } //for k end
          fire[i].particle[j][0].x += fire[i].particle[j][0].xs / 20;
          fire[i].particle[j][0].y += fire[i].particle[j][0].ys / 20;
          fire[i].particle[j][0].z += fire[i].particle[j][0].zs / 20;
          // update y speed
          float yd = gravity;
          fire[i].particle[j][0].ys += yd;
          // update x speed
          float xd = wind;
          fire[i].particle[j][0].xs += xd;
        }
      }
    }
    else{
      for (j = 0; j < MAX_PARTICLES; j++) {
         for(k = 0; k < MAX_TAIL; k++) {
            float x = fire[i].particle[j][k].x;
            float y = fire[i].particle[j][k].y;
            float z = fire[i].particle[j][k].z + zoom;
            float dt = 1 - (float)k/MAX_TAIL;

            glColor4f(fire[i].particle[j][0].r,
              fire[i].particle[j][0].g,
              fire[i].particle[j][0].b,
              fire[i].life*dt);
            glBegin(GL_QUADS);   // Build Quad From A Quadrangle
              glNormal3f(0.0f, 0.0f, 1.0f);
              glTexCoord2f(x+size, y+size);
              glVertex3f(x+size,y+size,z); // Top Right
              glTexCoord2f(x-size, y+size);
              glVertex3f(x-size,y+size,z); // Top Left
              glTexCoord2f(x+size, y-size);
              glVertex3f(x+size,y-size,z); // Bottom Right
              glTexCoord2f(x-size, y-size);
              glVertex3f(x-size,y-size,z); // Bottom Left
            glEnd(); // Done Building Triangle Strip
         }

         // update position
         for(k = MAX_TAIL-1; k > 0; k--) {
            fire[i].particle[j][k].x = fire[i].particle[j][k-1].x;
            fire[i].particle[j][k].y = fire[i].particle[j][k-1].y;
            fire[i].particle[j][k].z = fire[i].particle[j][k-1].z;
         } //for k end

         fire[i].particle[j][0].x += fire[i].particle[j][0].xs / 20;
         fire[i].particle[j][0].y += fire[i].particle[j][0].ys / 20;
         fire[i].particle[j][0].z += fire[i].particle[j][0].zs / 20;

         // update y speed
         float yd = gravity;
         fire[i].particle[j][0].ys += yd;
         float xd = wind;
         fire[i].particle[j][0].xs += xd;
      }
    }
    if(fire[i].up == 1 && fire[i].particle[0][0].ys < -gravity){
      fire[i].up = 0;
      for(j = 0; j < MAX_PARTICLES; j++){
        int color = rand()%12;
        fire[i].particle[j][0].r = colors[color][0];
        fire[i].particle[j][0].g = colors[color][1];
        fire[i].particle[j][0].b = colors[color][2];

        // x-z speed
        double radian = 3.14159*j*15.0/180.0;
        fire[i].particle[j][0].xs = (float)(fire[i].rad*sin(radian)) + wind;
        fire[i].particle[j][0].zs = (float)(fire[i].rad*cos(radian));
      }
    }

    fire[i].life -= fire[i].fade;
    if (fire[i].life < 0) {
      resetFire(i);
    }
  }

  glEnable(GL_DEPTH_TEST);
  //calculate FPS
  clock_gettime(CLOCK_MONOTONIC, &t2);
  fps++;
  if(t2.tv_sec - t1.tv_sec>0){
    clock_gettime(CLOCK_MONOTONIC, &t1);
    FPS_count++;
    FPS_sum += fps;
    if(FPS_count == 1)
      FPS_init = fps;
    // fprintf(fp2, "FPS: %f\n",fps );
    fps = 0;
  }

  if(print_count<500)
    // fprintf(fp1, "i = %d, p_x= %f, p_y = %f, p_z = %f, v_x= %f, v_y = %f, v_z = %f, Δv_x = %f, Δv_y = %f\n\n",
    //             print_count, fire[0].particle[0][0].x, fire[0].particle[0][0].y, fire[0].particle[0][0].z,
    //             fire[0].particle[0][0].xs / 20,
    //             fire[0].particle[0][0].ys / 20,
    //             fire[0].particle[0][0].zs / 20,
    //             wind,    gravity);
  if(print_flag == 1)
    print_count++;
  if(print_count == 500)
    print_flag = 0;

}

void menu (int menuentry) {
  switch (menuentry) {
  case 1: current_view= TOP_VIEW;
          break;
  case 2: current_view= ECLIPTIC_VIEW;
          break;
  case 3: current_view= SHIP_VIEW;
          break;
  }
}

void setView (void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (current_view) {
  case TOP_VIEW:
    gluLookAt(0.0, 60, 1.0,  // camera position
            0.0, 0.0, 0.0,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
    move=0;
    break;
  case ECLIPTIC_VIEW:
  gluLookAt(1, 0.0, 80,  // camera position
            0.0, 0.0, 0.0,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
    move=0;
    break;
  case SHIP_VIEW:
  gluLookAt(eyex, eyey, eyez,  // camera position
            centerx, centery, centerx,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
    move=1;
    break;
    glutPostRedisplay();
  }
}

void calculate_lookpoint(void) { /* Given an eyepoint and latitude and longitude angles, will
     compute a look point one unit away */
    centerx = eyex + cos((lat+mlat)*DEG_TO_RAD)*sin((lon+mlon)*DEG_TO_RAD);
    centery = eyey + sin((lat+mlat)*DEG_TO_RAD);
    centerz = eyez + cos((lat+mlat)*DEG_TO_RAD)*cos((lon+mlon)*DEG_TO_RAD);
} // calculate_lookpoint()

void display()
{
  glLoadIdentity();
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  setView();
  calculate_lookpoint(); /* Compute the centre of interest   */
  if(axisEnabled) glCallList(axisList);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo->bmiHeader.biWidth,
               TexInfo->bmiHeader.biHeight, 0, GL_BGR_EXT,
               GL_UNSIGNED_BYTE, TexBits);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  DrawGLScene();
  glutSwapBuffers();
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'q':
    case 'Q':     var_speed += 2;     break;
    case 'a':
    case 'A':     var_speed -= 2;     break;
    case 'w':
    case 'W':     var_life += 0.2;     break;
    case 's':
    case 'S':     var_life -= 0.2;     break;
    case 'r':
    case 'R':
      wind += 0.02;
      // fprintf(fp1, "\ngravity = %f, wind = %f\n", gravity, wind );
      print_flag = 1; print_count = 0;
      break;
    case 'f':
    case 'F':
      wind -= 0.02;
      // fprintf(fp1, "\ngravity = %f, wind = %f\n", gravity, wind );
      print_flag = 1; print_count = 0;
      break;
    case '1':      menu(1);      break;
    case '2':      menu(2);      break;
    case '3':      menu(3);      break;
    case ',':  //why move left&backwards?
      if(move){
        if(lon+TURN_ANGLE>90) lon=89.9;
        else                  lon+=TURN_ANGLE ;
        // printf("centerx=%f centery=%f centerz=%f lon=\n",centerx, centery, centerz );
      }
      break;
    case '.':  //why move right&backwards?
      if(move){
        if(lon-TURN_ANGLE<-90) lon=-89.9;
        else                  lon-=TURN_ANGLE ;
        // printf("centerx=%f centery=%f centerz=%f\n",centerx, centery, centerz );
      }
      break;
    case 'z':
    case 'Z':
      printf("before:eff_switch = %d\n", eff_switch);
      if(eff_switch == 1)
        eff_switch = 0;
      else
        eff_switch = 1;
      // eff_switch = 1 - eff_switch;
      // fprintf(fp2, "after:eff_switch = %d\n", eff_switch);    break;
    /* Escape key */
    case 27:
      FPS_count--;
      // fprintf(fp2, "average fps: %d\n",(FPS_sum-FPS_init)/FPS_count);
      exit(0);            break;
   }
}

void cursor_keys(int key, int x, int y) {
  if(move){
    switch (key) {
      case GLUT_KEY_LEFT: //left
      eyex += sin((lon+mlon+90)* DEG_TO_RAD) * RUN_SPEED;
      eyez += cos((lon+mlon+90)* DEG_TO_RAD) * RUN_SPEED;
      break;
      case GLUT_KEY_RIGHT://right
      eyex += sin((lon+mlon-90)* DEG_TO_RAD) * RUN_SPEED;
      eyez += cos((lon+mlon-90)* DEG_TO_RAD) * RUN_SPEED;
      break;
      case GLUT_KEY_PAGE_UP://up
      eyey += RUN_SPEED;
      break;
      case GLUT_KEY_PAGE_DOWN://down
      eyey -= RUN_SPEED;
      break;
      case GLUT_KEY_UP://forward
      eyex += sin(lon* DEG_TO_RAD) *RUN_SPEED;
      eyez += cos(lon* DEG_TO_RAD) * RUN_SPEED;
      break;
      case GLUT_KEY_DOWN://backward
      eyex -= sin(lon* DEG_TO_RAD) * RUN_SPEED;
      eyez -= cos(lon* DEG_TO_RAD) * RUN_SPEED;
      break;
    }
  }
} // cursor_keys()

///////////////////////////////////////////////
void reshape(int width, int height)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////
void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  TexBits = LoadDIBitmap("splodge.bmp", &TexInfo);
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  /* Define background colour */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  if(axis_flag)
    makeAxes();
  /* Set initial view parameters */
  eyex= 10;   eyey=  10;   eyez= 10;
  centerx = 0; centery = 0; centerz = 0;
  lat= 0.0;    lon= 0.0;
  mlat= 0.0;   mlon= 0.0;
  for(int i = 0; i < MAX_FIRE; i++) {
    resetFire(i);
  }//for i end
  // fprintf(fp1, "gravity = %f, wind = %f\n", gravity, wind );
}

/////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  // fp1 = fopen("motion.txt","w");
  // fp2 = fopen("FPS_eff.txt","w");
  clock_gettime(CLOCK_MONOTONIC, &t1);
  double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  // glutIdleFunc(tick);
  glEnable(GL_POINT_SMOOTH);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc (cursor_keys);
  glutMainLoop();
  // fclose(fp1);
  // fclose(fp2);
}
