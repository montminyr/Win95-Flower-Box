/*
 * Ryan Montminy - 6171235
 * Andrew Kornelsen - 6401145
 * COSC 3P98 - Final Project
 *
 *
 *
 * Features:
 * - 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <vector>
#include <time.h>

 /* Custom structures */

//The pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

//The global structure
typedef struct {
	pixel* data;
	int w, h, rotateSpeed, randMaxDir;
	bool  enableSpeedMod, enableFrictionMod, enableSprayMode;
	float camx, camy, camz;
} glob;



/* Global Variables */
int camDist = 0; // Move camera forward/backward (W / S)
int camX = 0; // 
int camY = 0;
int camAngleX = 0; // Camera angle rotating around X
int camAngleY = 0;// Camera angle rotating around Y

float speed = 0.5;
float tX = speed, tY = speed;
float posX, posY;
int color[3] = { 255,0,0 }; //Color of the object
int height = 45, width = 55; //Size of canvas
bool fixed = false; //Wether we are using a fixed color
int shape = 0; //Which shape we are currently using, defaults to "DVD" text

glob global; // Global
enum { MENU_QUIT }; // Menu options



/* Time progression */
void timer(int extra) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}


/* Disbale all objects and environment to display */
void display(void) {
	int p[][3] = { {6,6,6}, {6,-6,6}, {-6,-6,6}, {-6,6,6},
			   {6,6,-6}, {6,-6,-6}, {-6,-6,-6}, {-6,6,-6} };

	int e[][4] = { {0,3,2,1},{3,7,6,2},{7,4,5,6},{4,0,1,5}, {0,4,7,3},{1,2,6,5} };
	float c[][3] = { {1.0,0,0},{0,1.0,0},{1.0,1.0,1.0},
		   {0,0,1.0},{.6,0,.6},{0,.6,.6} };
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80, (float)(global.w / global.h), 0.5, 200);
	gluLookAt(global.camx, global.camy, global.camz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Camera movement
	glTranslatef(camDist, 0, camDist);
	glRotatef(camAngleY, 1, 0, 0);
	glRotatef(camAngleX, 0, 1, 0);

	

	// Decide how we are moving the object. Only changes if a side is hit.
	// Left bounce
	if (posX <= -width) tX = speed;
	// Right bounce
	if (posX >= width) tX = -speed;
	// Bottom Bounce
	if (posY <= -height) tY = speed;
	// Top Bounce
	if (posY >= height)tY = -speed;
	// Move object
	posX += tX;
	posY += tY;
	
	glPushMatrix();
	glTranslatef(posX, posY, 0);
	glRotatef(posY, 1, 0, 0);
	glRotatef(posX, 0, 1, 0);
	glRotatef(posY, 1, 0, 0);
	glRotatef(posX, 0, 0, 1);
	for (i = 0; i < 6; ++i) {
		glColor3fv(c[i]);
		glBegin(GL_QUADS);
		glVertex3iv(p[e[i][0]]);
		glVertex3iv(p[e[i][1]]);
		glVertex3iv(p[e[i][2]]);
		glVertex3iv(p[e[i][3]]);
		glEnd();
	}
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}


/* Moving camera using mouse. Only while mouse left click down */
void cameraMotion(int x, int y) {
	if (camX == 0) camX = x; //Set initial x
	if (camY == 0) camY = y; //Set initial y
	int diffX = camX - x; //x increment/decrement
	int diffY = camY - y; //y increment/decrement
	if (camAngleX < 360) camAngleX -= diffX; //Subtract for inverted movement
	else camAngleX = 0;
	if (camAngleY < 360) camAngleY -= diffY; //Subtract for inverted movement
	else camAngleY = 0;

	//Update x and y
	camX = x;
	camY = y;
}


/* Key bendings to print */
void show_keys() {
	printf("Q:Quit\nU:Unique Particles\nW:Forward\nS:Backward\nA:Rotate Left\nD:Rotate Right\nF:Shoot\nT:Increase Rotation Speed\nY:Decrese Rotation Speed\nZ:Toggle Speed Modifer\nC:Toggle Friction Mode\nX:Toggle Spray Mode\nR:Reset\n");
}


//Glut menu callback function
void menuFunc(int value) {
	switch (value) {
	case MENU_QUIT:
		exit(0);
		break;
	}
}//menuFunc


//Glut menu options
void init_menu() {
	int main_menu = glutCreateMenu(&menuFunc);
	glutAddMenuEntry("Quit", MENU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



/* Intialize Program */
int main(int argc, char** argv) {
	time_t t;
	srand((unsigned)time(&t));
	//Initial Window 
	global.w = 720;
	global.h = 720;
	global.camx = 0.0;
	global.camy = 0.0;
	global.camz = 70.0;
	global.rotateSpeed = 1;
	global.enableFrictionMod = false;
	global.enableSpeedMod = false;
	global.randMaxDir = 5;
	global.data = (pixel*)malloc(global.w * global.h * sizeof(pixel));

	// Fill the initial window
	for (int i = 0; i < global.w * global.h; i++) {
		global.data[i].r = 0;
		global.data[i].g = 0;
		global.data[i].b = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(global.w, global.h);
	glutCreateWindow("Particle Cannon"); //Window title

	//Lighting and Effects
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 1); // Set background to white
	glClear(GL_COLOR_BUFFER_BIT);

	//glutKeyboardFunc(keyboard); //Keyboard bindings
	glutMotionFunc(cameraMotion); //Camera movement
	glutDisplayFunc(display); //Display
	glutTimerFunc(0, timer, 0); //Time

	init_menu();
	show_keys(); //Print key binds to console
	glutMainLoop(); //Loop

	return 0; //Exit
}
