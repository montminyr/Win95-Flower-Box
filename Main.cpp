/*
 * Ryan Montminy - 6171235
 * Andrew Kornelsen - 6401145
 * COSC 3P98 - Final Project
 * 
 * Win95 Flower Box animation, done by vector morphing a cube.
 *
 * Key Details:
 * - Variable number of triangles per face, for finer details when morphing.
 * - Cube moves around the scene by bouncing off the walls in random directions.
 * - Morphing done by geometric concepts of normalization and linear interpolation.
 */

#include <freeglut.h>
#include <vector>
#include <time.h>

 /* Custom structures */
typedef struct {
	GLubyte r, g, b;
} pixel;
typedef struct Point {
	Point() {

	}
	Point(float inX, float inY, float inZ) {
		x = inX;
		y = inY;
		z = inZ;
	}
	float x, y, z;
}Point;
typedef struct Triangle {
	Triangle() {

	}
	Triangle(Point inA, Point inB, Point inC) {
		a = inA;
		b = inB;
		c = inC;
	}
	Point a, b, c;
}Triangle;
typedef struct Square {
	Square() {

	}
	Square(Point inA, Point inB, Point inC, Point inD) {
		a = inA;
		b = inB;
		c = inC;
		d = inD;
	}
	Point a, b, c, d;
}Square;
typedef struct {
	pixel* data;
	int w, h;
	bool  enableSpeedMod, enableFrictionMod, enableSprayMode;
	float camx, camy, camz;
} glob;


/* Global Variables */
float speed = 0.5; //Speed, to change from negative or postive
float animSpeed = 0.01; //Cube animation speed
float tX = speed, tY = speed; //Translate speed
float posX = 0, posY = 0; //Translate pos
int height = 45, width = 55; //Translation dimensions
bool toggle = false; //
glob global; // Global
enum { MENU_QUIT }; // Menu options
Point Normalize(Point a, Point b, int length);
std::vector<Triangle> triangles[] = { {},{},{},{},{},{} }; // Original cube, made up of triangles


/* Functions */


/* Time progression */
void timer(int extra) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

/* Linear interpolation, used to create an estimated point between first and second point.  */
Point lerp(Point x, Point y, float a) {
	Point c = { (x.x + (y.x - x.x) * a), (x.y + (y.y - x.y) * a), (x.z + (y.z - x.z) * a) };
	return c;
}

/* Normalize 3D vector, cube shape to sphercical by changing magnitude.
   Original point to point length is used to montain shape size */
Point Normalize(Point a, Point b, int length) {
	// get the distance between the two points
	float distX = b.x - a.x;
	float distY = b.y - a.y;
	float distZ = b.z - a.z;

	float PointDist = sqrt((distX * distX) + (distY * distY) + (distZ * distZ));

	// Normalize so that the distance between the points is equal to the desired length
	distX = distX * length / PointDist;
	distY = distY * length / PointDist;
	distZ = distZ * length / PointDist;

	// create the new point
	Point normalPoint;
	normalPoint.x = a.x + distX;
	normalPoint.y = a.y + distY;
	normalPoint.z = a.z + distZ;

	return normalPoint;
}

/* Caclulates mid point of a two Points, useful for creating x num triangles */
Point Mid(Point a, Point b) {
	Point c = { (a.x + b.x) / 2,(a.y + b.y) / 2,(a.z + b.z) / 2 };
	return c;
}

/* Disbale all objects and environment to display */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80, (float)(global.w / global.h), 0.5, 200);
	gluLookAt(global.camx, global.camy, global.camz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


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
	// Clamp the  animation to the constraint of the original Win95 FlowerBox
	float animation = -1 + animSpeed; // shape morphing clamp from -1 -> 5
	if (animation >= 5){
		toggle = true;
	}
	if (animation <= -1) {
		toggle = false;
	}

	//Speed of morphing, and direction of morphing animation
	if (toggle) {
		animSpeed -= 0.03; //reverse
	}
	else {
		animSpeed += 0.03; //forward
	}

	//Colors of each face
	int color[][3] = { {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {0, 255, 255}, {255, 0, 255} };


	glPushMatrix();

	//move,rotate, and bounce off sides
	glTranslatef(posX, posY, 0);
	glRotatef(posY, 1, 0, 0);
	glRotatef(posX, 0, 1, 0);
	glRotatef(posY, 1, 0, 0);
	glRotatef(posX, 0, 0, 1);

	Point origin = { 0,0,0 };
	for (int i = 0; i < 6; i++) { // 6 faces
		for (int j = 0; j < triangles[i].size(); j++) { // num of triangles per side
			glColor3f(color[i][0], color[i][1], color[i][2]);
			glBegin(GL_TRIANGLES);
			Point tempA = lerp(triangles[i][j].a, Normalize(origin, triangles[i][j].a, 7.5), animation); //Point 1
			Point tempB = lerp(triangles[i][j].b, Normalize(origin, triangles[i][j].b, 7.5), animation); //Point 2
			Point tempC = lerp(triangles[i][j].c, Normalize(origin, triangles[i][j].c, 7.5), animation); //Point 3

			glVertex3f(tempA.x, tempA.y, tempA.z);
			glVertex3f(tempB.x, tempB.y, tempB.z);
			glVertex3f(tempC.x, tempC.y, tempC.z);
			glEnd();
		}
	}
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

/* Glut menu callback function */
void menuFunc(int value) {
	switch (value) {
	case MENU_QUIT:
		exit(0);
		break;
	}
}//menuFunc

/* Glut menu options */
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
	global.w = 800;
	global.h = 800;
	global.camx = 0.0;
	global.camy = 0.0;
	global.camz = 70.0;
	global.enableFrictionMod = false;
	global.enableSpeedMod = false;
	global.data = (pixel*)malloc(global.w * global.h * sizeof(pixel));

	// Fill the initial window
	for (int i = 0; i < global.w * global.h; i++) {
		global.data[i].r = 0;
		global.data[i].g = 0;
		global.data[i].b = 0;
	}

	//All Points used to make inital cube shape
	int scale = 15; //size of cube lengths
	float div = scale / 2; //used to center around origin 0,0
	Square CubeFaces[] = { Square(Point(-div,div,div), Point(div,div,div), Point(div,-div,div), Point(-div,-div,div)), //Face 1
						   Square(Point(div,div,div), Point(div,div,-div), Point(div,-div,-div), Point(div,-div,div)), //Face 2
						   Square(Point(div,div,-div), Point(-div,div,-div), Point(-div,-div,-div), Point(div,-div,-div)), //Face 3
						   Square(Point(-div,div,-div), Point(-div,div,div), Point(-div,-div,div), Point(-div,-div,-div)), //Face 4
						   Square(Point(-div,div,-div), Point(div,div,-div), Point(div,div,div), Point(-div,div,div)), //Face 5
						   Square(Point(-div,-div,-div), Point(div,-div,-div), Point(div,-div,div), Point(-div,-div,div)) }; //Face 6


	//Remake cube using triangles
	for (int i = 0; i < 6; i++) {
		triangles[i].push_back(Triangle(CubeFaces[i].a, CubeFaces[i].c, CubeFaces[i].b));
		triangles[i].push_back(Triangle(CubeFaces[i].a, CubeFaces[i].c, CubeFaces[i].d));
		for (int j = 0; j < 10; j++) { // 2^10 triangles
			int size = triangles[i].size();
			for (int k = 0; k < size; k++) {
				Triangle temp = triangles[i][k];
				triangles[i].push_back(Triangle(temp.a, temp.c, Mid(temp.a, temp.b)));
				triangles[i].push_back(Triangle(temp.b, temp.c, Mid(temp.a, temp.b)));
			}
			for (int k = 0; k < size; k++) triangles[i].erase(triangles[i].begin()); //Remove triangles used to make new triangles
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(global.w, global.h);
	glutCreateWindow("Win95 3D Flower Box Screensaver");
	glutFullScreen();

	//Lighting and Effects
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glutDisplayFunc(display); //Display
	glutTimerFunc(0, timer, 0); //Time

	init_menu();
	glutMainLoop(); //Loop

	return 0; //Exit
}