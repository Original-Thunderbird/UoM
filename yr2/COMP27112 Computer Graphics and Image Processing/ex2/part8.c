/*
==========================================================================
File:        ex2.c (skeleton)
Version:     5, 19/12/2017
Author:     Toby Howard
==========================================================================
*/

/* The following ratios are not to scale: */
/* Moon orbit : planet orbit */
/* Orbit radius : body radius */
/* Sun radius : planet radius */

#ifdef MACOS
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BODIES 25
#define TOP_VIEW 1
#define ECLIPTIC_VIEW 2
#define SHIP_VIEW 3
#define EARTH_VIEW 4
#define PI 3.141593
#define DEG_TO_RAD 0.01745329
#define ORBIT_POLY_SIDES 50
#define TIME_STEP 0.5   /* days per frame */
#define RUN_SPEED  5000000
#define TURN_ANGLE 4.0

typedef struct {
  char    name[25];       /* name */
  GLfloat r,g,b;          /* colour */
  GLfloat orbital_radius; /* distance to parent body (km) */
  GLfloat orbital_tilt;   /* angle of orbit wrt ecliptic (deg) */
  GLfloat orbital_period; /* time taken to orbit (days) */
  GLfloat radius;         /* radius of body (km) */
  GLfloat axis_tilt;      /* tilt of axis wrt body's orbital plane (deg) */
  GLfloat rot_period;     /* body's period of rotation (days) */
  GLint   orbits_body;    /* identifier of parent body */
  GLfloat spin;           /* current spin value (deg) */
  GLfloat orbit;          /* current orbit value (deg) */
 } body;

GLdouble lat,     lon;              /* View angles (degrees)    */
GLdouble mlat,    mlon;             /* Mouse look offset angles */
GLfloat  eyex,    eyey,    eyez;    /* Eye point                */
GLfloat  centerx, centery, centerz; /* Look point               */

body  bodies[MAX_BODIES];
int   numBodies, current_view, draw_orbits, draw_labels, draw_starfield, axis_flag=1, move;
float  date;

/*****************************/
float myRand (void){
  /* return a random float in the range [0,1] */
  return (float) rand() / RAND_MAX;
}
/*****************************/
float star_x[1000],star_y[1000],star_z[1000];
void drawStarfield (void)
{
  short i;
  glBegin(GL_POINTS);
  for(i=0;i<1000;i++){
    glColor3f(1.0,1.0,1.0); // white
    glVertex3f(star_x[i],star_y[i],star_z[i]);
    //printf("%f, %f, %f\n",star_x[i],star_y[i],star_z[i]);
  }
  glEnd();
  /* This is for you to complete. */
}

/*****************************/

void readSystem(void)
{
  /* reads in the description of the solar system */

  FILE *f;
  int i;

  f= fopen("sys", "r");
  if (f == NULL) {
     printf("ex2.c: Couldn't open 'sys'\n");
     printf("To get this file, use the following command:\n");
     printf("  cp /opt/info/courses/COMP27112/ex2/sys .\n");
     exit(0);
  }
  fscanf(f, "%d", &numBodies);
  for (i= 0; i < numBodies; i++)
  {
    fscanf(f, "%s %f %f %f %f %f %f %f %f %f %d",
      bodies[i].name,
      &bodies[i].r, &bodies[i].g, &bodies[i].b,
      &bodies[i].orbital_radius,
      &bodies[i].orbital_tilt,
      &bodies[i].orbital_period,
      &bodies[i].radius,
      &bodies[i].axis_tilt,
      &bodies[i].rot_period,
      &bodies[i].orbits_body);

    /* Initialise the body's state */
    bodies[i].spin= 0.0;
    bodies[i].orbit= myRand() * 360.0; /* Start each body's orbit at a
                                          random angle */
    bodies[i].radius*= 1000.0; /* Magnify the radii to make them visible */
  }
  fclose(f);
}

/*****************************/

void drawString (void *font, float x, float y, char *str)
{ /* Displays the string "str" at (x,y,0), using font "font" */

  /* This is for you to complete. */

}

/*****************************/

void setView (void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (current_view) {
  case TOP_VIEW:
    gluLookAt(0.0, bodies[0].radius*40, 1.0,  // camera position
            0.0, 0.0, 0.0,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
    move=0;
    break;
  case ECLIPTIC_VIEW:
  gluLookAt(1, 0.0, bodies[0].radius*28,  // camera position
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
  case EARTH_VIEW:
  gluLookAt(bodies[3].orbital_radius*cos(bodies[3].orbit*DEG_TO_RAD),bodies[3].radius*1.2,bodies[3].orbital_radius*sin(bodies[3].orbit*DEG_TO_RAD),  // camera position
            0.0, 0.0, 0.0,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
    move=0;
    break;
    glutPostRedisplay();
  }
}

/*****************************/

void menu (int menuentry) {
  switch (menuentry) {
  case 1: current_view= TOP_VIEW;
          break;
  case 2: current_view= ECLIPTIC_VIEW;
          break;
  case 3: current_view= SHIP_VIEW;
          break;
  case 4: current_view= EARTH_VIEW;
          break;
  case 5: draw_labels= !draw_labels;
          break;
  case 6: draw_orbits= !draw_orbits;
          break;
  case 7: draw_starfield= !draw_starfield;
          break;
  case 8: exit(0);
  }
}

/*****************************/

void init(void)
{
  /* Define background colour */
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Top view", 1);
  glutAddMenuEntry ("Ecliptic view", 2);
  glutAddMenuEntry ("Spaceship view", 3);
  glutAddMenuEntry ("Earth view", 4);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Toggle labels", 5);
  glutAddMenuEntry ("Toggle orbits", 6);
  glutAddMenuEntry ("Toggle starfield", 7);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Quit", 8);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  current_view= TOP_VIEW;
  draw_labels= 1;
  draw_orbits= 1;
  draw_starfield= 1;

  eyex=13920*1000*8, eyey=13920*1000*8, eyez=13920*1000*8;
  centerx=0, centery=0, centerz=0;
  lat= 0.0;   /* Look horizontally ...  */
  lon= 0.0;   /* ... along the +Z axis  */

  short i;
  float base=13920*1000*30;
  for(i=0;i<1000;i++){
    star_x[i]=2*base*myRand()-base;
    star_y[i]=2*base*myRand()-base;
    star_z[i]=2*base*myRand()-base;
  }
}

/*****************************/
void calculate_lookpoint(void) { /* Given an eyepoint and latitude and longitude angles, will
     compute a look point one unit away */
    centerx = eyex + 13920*1000*8*cos(lat*DEG_TO_RAD)*sin(lon*DEG_TO_RAD);
    centery = eyey + 13920*1000*8*sin(lat*DEG_TO_RAD);
    centerz = eyez + 13920*1000*8*cos(lat*DEG_TO_RAD)*cos(lon*DEG_TO_RAD);
} // calculate_lookpoint()
/*****************************/

void animate(void){
  int i;
    date+= TIME_STEP;
    for (i= 0; i < numBodies; i++)  {
      bodies[i].spin += 360.0 * TIME_STEP / bodies[i].rot_period;
      bodies[i].orbit += 360.0 * TIME_STEP / bodies[i].orbital_period;
      glutPostRedisplay();
    }
}

/*****************************/

void drawOrbit (int n)
{ /* Draws a polygon to approximate the circular
     orbit of body "n" */
     glBegin(GL_LINES);
     glColor3f(bodies[n].r,bodies[n].g,bodies[n].b); // red
     short i;
     for(i=0;i<720;i++){
       glVertex3f(bodies[n].orbital_radius*cos(i*0.5*DEG_TO_RAD),0,bodies[n].orbital_radius*sin(i*0.5*DEG_TO_RAD));
       glVertex3f(bodies[n].orbital_radius*cos((i+1)*0.5*DEG_TO_RAD),0,bodies[n].orbital_radius*sin((i+1)*0.5*DEG_TO_RAD));
       //printf("%f, %f, %f\n",star_x[i],star_y[i],star_z[i]);
     }
     glEnd();
    /* This is for you to complete. */
}

/*****************************/

void drawLabel(int n)
{ /* Draws the name of body "n" */

    /* This is for you to complete. */
}

/*****************************/

void drawBody(int n){
  //if(n==0 || n==1){
    //apply transformation first, draw body later
    if(bodies[n].orbits_body !=0){
      glRotatef(bodies[bodies[n].orbits_body].orbital_tilt,0,0,1); //set orbital_tilt-------why use z-axis instead of y-axis as param?
      glTranslatef(bodies[bodies[n].orbits_body].orbital_radius*cos(bodies[bodies[n].orbits_body].orbit*DEG_TO_RAD),0,bodies[bodies[n].orbits_body].orbital_radius*sin(bodies[bodies[n].orbits_body].orbit*DEG_TO_RAD));//translation along orbit
    }
    glRotatef(bodies[n].orbital_tilt,0,0,1); //set orbital_tilt-------why use z-axis instead of y-axis as param?


    drawOrbit(n);
    // if(bodies[n].orbits_body !=0)
    //   glTranslatef(bodies[bodies[n].orbits_body].orbital_radius*cos(bodies[bodies[n].orbits_body].orbit*DEG_TO_RAD),0,bodies[bodies[n].orbits_body].orbital_radius*sin(bodies[bodies[n].orbits_body].orbit*DEG_TO_RAD));//translation along orbit
    // else
      glTranslatef(bodies[n].orbital_radius*cos(bodies[n].orbit*DEG_TO_RAD),0,bodies[n].orbital_radius*sin(bodies[n].orbit*DEG_TO_RAD));//translation along orbit

    glRotatef(bodies[n].axis_tilt,0,0,1); //set axis_tilt---------why use z-axis instead of y-axis as param?
    glRotatef(bodies[n].spin*DEG_TO_RAD,0,1,0);//spin aournd body's axis
    glColor3f(bodies[n].r,bodies[n].g,bodies[n].b); // color
    glBegin(GL_LINES);
    glColor3f(bodies[n].r,bodies[n].g,bodies[n].b); // color
    glVertex3f(0.0, bodies[n].radius*1.4, 0.0);
    glVertex3f(0.0, -bodies[n].radius*1.4, 0.0);
    glEnd();
    glutSolidTorus(0.02,5.0,10,64);
    //only rotate sphere, so axis s drawn in advance
    glRotatef(90,1,0,0);//rorate sphere
    glutWireSphere (bodies[n].radius, 15, 15);    //draw
  //}
 /* Draws body "n" */
}

/*****************************/
void display(void){
  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  glTranslatef(1.0, 0.0, 0.0);
  //part1-axis
  float LEN= 100000000.0;
  if(axis_flag){
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0); // red,x-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(LEN, 0.0, 0.0);
    glColor3f(0.0,1.0,0.0); // green,y-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, LEN, 0.0);
    glColor3f(0.0,0.0,1.0); // blue,z-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, LEN);
    glEnd();
    glLineWidth(1.0);
  }

  /* set the camera */
  setView();
  calculate_lookpoint(); /* Compute the centre of interest   */
  if (draw_starfield) drawStarfield();

  for (i= 0; i < numBodies; i++)
  {
    glPushMatrix();
      drawBody (i);
    glPopMatrix();
  }

  glutSwapBuffers();
}

/*****************************/

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (48.0, (GLfloat)w/(GLfloat)h,  10000.0, 8000000000.0);
}

/*****************************/

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'A':
    case 'a':      axis_flag=1-axis_flag;      break;
    case '1':      menu(1);      break;
    case '2':      menu(2);      break;
    case '3':      menu(3);      break;
    case '4':      menu(4);      break;
    case ',':  //why move left&backwards?
      if(move){
        eyex += sin((lon+90)* DEG_TO_RAD) * RUN_SPEED;
        eyez += cos((lon+90)* DEG_TO_RAD) * RUN_SPEED;
      }
      break;
    case '.':  //why move right&backwards?
      if(move){
        eyex += sin((lon-90)* DEG_TO_RAD) * RUN_SPEED;
        eyez += cos((lon-90)* DEG_TO_RAD) * RUN_SPEED;
      }
      break;
    case 27:  /* Escape key */
      exit(0);
  }
}

/*****************************/
void cursor_keys(int key, int x, int y) {
  if(move){
    switch (key) {
      case GLUT_KEY_LEFT:
        if(lon+TURN_ANGLE>90) lon=89.9;
        else                  lon+=TURN_ANGLE ;
        break;
      case GLUT_KEY_RIGHT:
        if(lon-TURN_ANGLE<-90) lon=-89.9;
        else                  lon-=TURN_ANGLE ;
        break;
      case GLUT_KEY_PAGE_UP:
      eyey += RUN_SPEED;
      break;
      case GLUT_KEY_PAGE_DOWN:
      eyey -= RUN_SPEED;
      break;
      case GLUT_KEY_UP:
      eyex += sin(lon* DEG_TO_RAD) *RUN_SPEED;
      eyez += cos(lon* DEG_TO_RAD) * RUN_SPEED;
      break;
      case GLUT_KEY_DOWN:
      eyex -= sin(lon* DEG_TO_RAD) * RUN_SPEED;
      eyez -= cos(lon* DEG_TO_RAD) * RUN_SPEED;
      break;
    }
  }
} // cursor_keys()
/*****************************/

int main(int argc, char** argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow ("COMP27112 Exercise 2");
  //glutFullScreen();
  init();
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutSpecialFunc (cursor_keys);
  glutIdleFunc (animate);
  readSystem();
  glutMainLoop();
  return 0;
}
/* end of ex2.c */