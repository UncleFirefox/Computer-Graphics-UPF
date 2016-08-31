// Practica 4 Infografia I by Javi Agenjo. 2008

//includes
#ifdef WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include "framework.h"
#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//The model matrix
Matrix33 model;

// Transformation functions
//***************************

//Resets the model
void loadIdentity()
{
	model.setIdentity();
}

void translateLocal(float x, float y)
{
	//apply the translation to the model matrix
	//...
	Matrix33 translation;
	translation.setTranslation(x,y);
	model.multiply(translation);

}

void rotateLocal(float angle)
{
	//apply the rotation to the model matrix
	//...
	Matrix33 rotation;
	rotation.setRotation(angle);
	model.multiply(rotation);
}

void scaleLocal(float x, float y)
{
	//apply the scalation to the model matrix
	//...
	Matrix33 scalation;
	scalation.setScale(x,y);
	model.multiply(scalation);
}

void translateWorld(float x, float y)
{
	//apply the translation to the model matrix
	//...
	Matrix33 translation;
	translation.setTranslation(x,y);
	translation.multiply(model);
	model = translation;
}

void rotateWorld(float angle)
{
	//apply the rotation to the model matrix
	//...
	Matrix33 rotation;
	rotation.setRotation(angle);
	rotation.multiply(model);
	model = rotation;
}

void scaleWorld(float x, float y)
{
	//apply the scalation to the model matrix
	//...
	Matrix33 scalation;
	scalation.setScale(x,y);
	scalation.multiply(model);
	model = scalation;
}

// *********************************

void drawAxis()
{
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);
	glBegin( GL_LINES );
		glVertex2f(-1000,0);
		glVertex2f(1000,0);
		glVertex2f(0,-1000);
		glVertex2f(0,1000);
	glEnd();
}

void drawTriangle()
{
	Point vertices[3];
	vertices[0].y = 30;
	vertices[1].x = -20;
	vertices[1].y = -20;
	vertices[2].x = 20;
	vertices[2].y = -20;

	//transform the vertices
	for (int i = 0; i < 3; i++)
		model.transform( vertices[i] );

	//cute triangle
	glColor3f(0.7,0.5,0.6);
	glBegin( GL_TRIANGLES );
		glVertex3fv( (float*)&vertices[0] );
		glVertex3fv( (float*)&vertices[1] );
		glVertex3fv( (float*)&vertices[2] );
	glEnd();
	glLineWidth(2);
	glColor3f(1,1,1);
	glBegin( GL_LINE_LOOP );
		glVertex3fv( (float*)&vertices[0] );
		glVertex3fv( (float*)&vertices[1] );
		glVertex3fv( (float*)&vertices[2] );
	glEnd();
}

//what to do when the image has to be draw
void onDraw(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAxis();

	drawTriangle();

	//swap between front buffer and back buffer
	glutSwapBuffers();
}


//what to do on a window reshape
static void onReshape(int w, int h)
{
	float range = w / 2.0f; //we want to match the system with the pixels

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
		glOrtho (-range, range, -range*h/w, range*h/w, -range, range);
    else 
		glOrtho (-range*w/h, range*w/h, -range, range, -range, range);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//what to do on a key event
void onKeyEvent(unsigned char key, int x, int y)
{
    switch (key) 
	{
		case 27: exit(0); break; //ESC

		//some interaction over here
		//...

		case 'a': translateLocal(-10.0f, 0.0f); break;
		case 'd': translateLocal(10.0f, 0.0f); break;
		case 'w': translateLocal(0.0f, 10.0f); break;
		case 's': translateLocal(0.0f, -10.0f); break;
		case 'z': rotateLocal(30.0f); break;
		case 'x': rotateLocal(-30.0f); break;
		case 'c': scaleLocal(5.0f, 5.0f); break;
		case 'v': scaleLocal(0.2f, 0.2f); break;



		case ' ': loadIdentity(); break;
	}
}


void init(void)
{
	loadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("My application");

    init();

    glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyEvent);
    glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
    glutMainLoop();
	return 0;
}
