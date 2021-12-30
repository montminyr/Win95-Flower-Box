/*
* 
* Ryan Montminy
* COSC 3P98 - Assignment 1 - Part 2
* 
* Screensaver to the likeness of the old DVD player animation.
* Bounces perfectly off the sides.
* 
* Features:
* - Fix color to R, G, or B.
* - Change shape from text, triangle square.
* - Speed up or slow down.
* - Changes color every bounce, if fixed not selected.
* - Quit screensaver.
* - Reset to default settings.
* 
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <GL/gl.h>
#include <string>


int speed = 2;
int tX = speed, tY = speed;
int posXOne = 225, posXTwo = 275, posYOne = 300, posYTwo = 350; //Curr Pos of the object
int color[3] = { 255,0,0 }; //Color of the object
int height = 400, width = 500; //Size of canvas
bool fixed = false; //Wether we are using a fixed color
int shape = 0; //Which shape we are currently using, defaults to "DVD" text

enum {
    MENU_QUIT, MENU_RESET, MENU_SDOWN, MENU_SUP, MENU_FRED, MENU_FGREEN, MENU_FBLUE, MENU_DEFAULT, MENU_TRIANGLE, MENU_SQUARE
};


// Renders a string on the glut display
// https://www.opengl.org/resources/libraries/glut/spec3/node76.html
void displayText(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(string); i++) {
        glutBitmapCharacter(font, string[i]);
    }
}

// Renders default image, and logic for screensaver movement.
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(color[0], color[1], color[2]);

    // Decide how we are moving the object. Only changes if a side is hit.
    if (shape == 0) {
        // Left bounce
        if (posXOne <= 0) {
            tX = speed;
            if (!fixed) {
                color[0] = 0, color[1] = 255, color[2] = 0;
            }
        }
        // Right bounce
        if (posXOne +10>= width) {
            tX = -speed;
            if (!fixed) {
                color[0] = 0, color[1] = 0, color[2] = 255;
            }
        }
        // Bottom bounce
        if (posYOne - 10 <= 0) {
            tY = speed;
            if (!fixed) {
                color[0] = 255, color[1] = 255, color[2] = 0;
            }
        }
        // Top bounce
        if (posYOne + 10 >= height) {
            tY = -speed;
            if (!fixed) {
                color[0] = 0, color[1] = 255, color[2] = 255;
            }
        }
    }
    else {
        // Left bounce
        if (posXOne <= 0 || posXTwo <= 0) {
            tX = speed;
            if (!fixed) {
                color[0] = 0, color[1] = 255, color[2] = 0;
            }
        }
        // Right bounce
        if (posXOne >= width || posXTwo >= width) {
            tX = -speed;
            if (!fixed) {
                color[0] = 0, color[1] = 0, color[2] = 255;
            }
        }
        // Bottom Bounce
        if (posYOne <= 0 || posYTwo <= 0) {
            tY = speed;
            if (!fixed) {
                color[0] = 255, color[1] = 255, color[2] = 0;
            }
        }
        // Top Bounce
        if (posYOne >= height || posYTwo >= height) {
            tY = -speed;
            if (!fixed) {
                color[0] = 0, color[1] = 255, color[2] = 255;
            }
        }
    }
    // Move object
    posXOne += tX;
    posXTwo += tX;
    posYOne += tY;
    posYTwo += tY;

    if (shape == 0) displayText(posXOne, posYOne, (void*)GLUT_BITMAP_TIMES_ROMAN_24, "DVD"); // DVD text
    if (shape == 1) { //Triangle
        glBegin(GL_TRIANGLES);
            glVertex2f(posXOne, posYOne);
            glVertex2f(posXTwo, posYOne);
            glVertex2f(posXOne, posYTwo);
        glEnd();
    }
    if (shape == 2) { //Square
        glBegin(GL_QUADS);
            glVertex2f(posXOne, posYTwo);
            glVertex2f(posXTwo, posYTwo);
            glVertex2f(posXTwo, posYOne);
            glVertex2f(posXOne, posYOne);
        glEnd();
    }
    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 'n': //Reset
    case 'N':
        color[0] = 255, color[1] = 0, color[2] = 0;
        fixed = false;
        speed = 2;
        tX = speed, tY = speed;
        posXOne = 225, posXTwo = 275, posYOne = 300, posYTwo = 350;
        shape = 0;
        glutPostRedisplay();
        break;
    case '-': //Slow down
        if (tX < 0) {
            tX = tX + 1;
            tY = tY + 1;
            speed -= 1;
        }
        if (tX > 0) {
            tX = tX - 1;
            tY = tY - 1;
            speed -= 1;
        }
        break;

    case '+': //Speed up
        if (tX < 0) {
            tX = tX - 1;
            tY = tY - 1;
            speed += 1;
        }
        if (tX >= 0) {
            tX = tX + 1;
            tY = tY + 1;
            speed += 1;
        }
        break;
    case 'r': //Red
    case 'R':
        fixed = true;
        color[0] = 255, color[1] = 0, color[2] = 0;
        break;
    case 'g': //Green
    case 'G':
        fixed = true;
        color[0] = 0, color[1] = 255, color[2] = 0;
        break;
    case 'b': //Blue
    case 'B':
        fixed = true;
        color[0] = 0, color[1] = 0, color[2] = 255;
        break;
    case 0x1B: //Quit
    case 'q':
    case 'Q':
        exit(0);
        break;
    case 'd': //Default shape
    case 'D':
        shape = 0;
        break;
    case 't': //Triangle
    case 'T':
        shape = 1;
        break;
    case 'x': //Square
    case 'X':
        shape = 2;
        break;
    }
}

void show_keys()
{
    printf("Q: Quit\nN: Reset\n+: Speed Up\n-: Slow Down\nR: Static Red Color\nG: Static Green Color\nB: Static Blue Color\nD: Default Shape\nT: Triangle Shape\nX: Square Shape");
}

void menuFunc(int value)
{
    switch (value) {
    case MENU_QUIT:
        exit(0);
        break;
    case MENU_RESET:
        color[0] = 255, color[1] = 0, color[2] = 0;
        fixed = false;
        speed = 2;
        tX = speed, tY = speed;
        posXOne = 225, posXTwo = 275, posYOne = 300, posYTwo = 350;
        shape = 0;
        glutPostRedisplay();
        break;
    case MENU_SUP:
        if (tX < 0) {
            tX = tX - 1;
            tY = tY - 1;
            speed += 1;
        }
        if (tX >= 0) {
            tX = tX + 1;
            tY = tY + 1;
            speed += 1;
        }
        break;
    case MENU_SDOWN:
        if (tX < 0) {
            tX = tX + 1;
            tY = tY + 1;
            speed -= 1;
        }
        if (tX > 0) {
            tX = tX - 1;
            tY = tY - 1;
            speed -= 1;
        }
        break;
    case MENU_FRED:
        fixed = true;
        color[0] = 255, color[1] = 0, color[2] = 0;
        break;
    case MENU_FGREEN:
        fixed = true;
        color[0] = 0, color[1] = 255, color[2] = 0;
        break;
    case MENU_FBLUE:
        fixed = true;
        color[0] = 0, color[1] = 0, color[2] = 255;
        break;
    case MENU_DEFAULT:
        shape = 0;
        break;
    case MENU_TRIANGLE:
        shape = 1;
        break;
    case MENU_SQUARE:
        shape = 2;
        break;
    }
}//menuFunc

void init_menu()
{
    int speed_menu = glutCreateMenu(&menuFunc);
    glutAddMenuEntry("Speed Up", MENU_SUP);
    glutAddMenuEntry("Slow Down", MENU_SDOWN);

    int fcolor_menu = glutCreateMenu(&menuFunc);
    glutAddMenuEntry("Fixed Red", MENU_FRED);
    glutAddMenuEntry("Fixed Green", MENU_FGREEN);
    glutAddMenuEntry("Fixed Blue", MENU_FBLUE);

    int fshape_menu = glutCreateMenu(&menuFunc);
    glutAddMenuEntry("Default", MENU_DEFAULT);
    glutAddMenuEntry("Triangle", MENU_TRIANGLE);
    glutAddMenuEntry("Square", MENU_SQUARE);

    int main_menu = glutCreateMenu(&menuFunc);
    glutAddSubMenu("Change Speed", speed_menu);
    glutAddSubMenu("Change Shape", fshape_menu);
    glutAddSubMenu("Fixed Color", fcolor_menu);
    glutAddMenuEntry("Reset", MENU_RESET);
    glutAddMenuEntry("Quit", MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("Screensaver");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

    init_menu();
    show_keys();

    glutFullScreen();
    glutMainLoop();
}