// Practica 3 Infografia I by Javi Agenjo. 2008

//includes
#ifdef WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include "framework.h"
#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

std::vector<Point> points_pressed;
std::vector<Line> lines;

ClippingRectangle clipping_rectangle;
bool rectangleSet = false;

void init(void)
{
}

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

//what to do when the image has to be draw
void onDraw(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAxis();

	glColor3f(1,0,0);
	clipping_rectangle.draw();

	//draw all the lines
	glColor3f(1,1,1);
	for (unsigned int i = 0; i < lines.size(); i++)
		lines[i].draw();

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
		case 27: //ESC key
		{
			exit(0);
			break;
		}
	}
}

void onMouseEvent(int button, int state, int x, int y)
{
	if ( button ==  GLUT_LEFT_BUTTON )
	{
		int  width = glutGet(GLUT_WINDOW_WIDTH);
		int  height = glutGet(GLUT_WINDOW_HEIGHT);
		if ( state == GLUT_DOWN )
		{
			//important to convert the coordinates from window coordinates to world coordinates
			Point last_point;
			last_point.x = x - width / 2;
			last_point.y = height - y - (height / 2);
			points_pressed.push_back(last_point);

			//if the point is one of the first two points then use them to set the clipping rectangle
			//...
			if (points_pressed.size() == 2 && !rectangleSet)
			{
				//Dibujamos el rectangulo
				clipping_rectangle.set(points_pressed[0],points_pressed[1]);
				lines.empty();
				rectangleSet = true;
			}
			else if (points_pressed.size() > 2 )
			{
				//Aqui computamos los puntos, computamos una linea por cada par de puntos agregados
				if (points_pressed.size() % 2 == 0)
				{
					Line linea_actual(points_pressed[points_pressed.size()-1], points_pressed[points_pressed.size()-2]);

					if (clipping_rectangle.clipLine(linea_actual))
					{
						lines.push_back(linea_actual);
					}
				}
			}
		}
	}
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
	glutMouseFunc(onMouseEvent);
    glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
    glutMainLoop();
	return 0;
}
