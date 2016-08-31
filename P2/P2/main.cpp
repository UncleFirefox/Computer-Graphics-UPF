//includes
#ifdef WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include "framework.h"
#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

Point last_point;
Point first_point;
bool isFirst = true;
std::vector<Point> puntos;
int drawMode = 0;


void init(void)
{
	//test line algorythms
	long start_time = getTime();

	Line my_line; //my test line

	for (int i = 0; i < 10000; i++) //for every line
	{
		my_line.start.Random( 200 ); //set a random start
		my_line.end.Random( 200 ); //Set a random end
		my_line.CalculePointsUsingBresenham(); //we dont grab the return because we dont care, it is just a benchmark
	}

	std::cout << "Milliseconds elapsed to calcule 10K lines: " << getTime() - start_time << std::endl;
}

void drawPoints(std::vector<Point> points)
{
	glColor3f(1,1,1);
	glPointSize(1);
	glBegin ( GL_POINTS );
		for (int i = 0; i < points.size(); i++)
			glVertex2f( points[i].x, points[i].y );
	glEnd();
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
	glPointSize(3);
	glBegin(GL_POINTS);
		glVertex2f( last_point.x, last_point.y );
	glEnd();

	//draw the scene
	drawPoints(puntos);

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
		case 32: //Space Key
		{
			if (drawMode < 2)
			{
				drawMode++;
			}
			else
			{
				drawMode = 0;
			}
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
			last_point.x = x - width / 2;
			last_point.y = height - y - (height / 2);

			//...

			Line recta;

			if (isFirst)
			{
				if (drawMode == 0)
				{
					recta.start = first_point;
					recta.end = last_point;
					puntos = recta.CalculePointsUsingBresenham();
				}
				else if (drawMode == 1)
				{
					recta.start = first_point;
					recta.end = last_point;
					puntos = recta.CalculePointsUsingDDA();
				}
				else if (drawMode == 2)
				{
					Circle circulo(first_point, last_point);
					puntos = circulo.CalculePointsUsingBresenham();
				}

				first_point = last_point;
			}
			else
			{
				first_point = last_point;
				isFirst = false;
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
