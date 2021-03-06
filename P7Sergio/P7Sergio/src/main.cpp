// Practica 4 Infografia I by Javi Agenjo. 2008

//includes
#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include "framework.h"
#include "mesh.h"
#include "camera.h"
#include "Textura.h"

#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//The model matrix
std::vector<Vector3> pressed_points;

Camera camera2D;
Camera camera3D;
Camera* current_camera;

Escenario e;
Suelo s;
Fuente f;
Estrellas est;
Estrellas polar;

// ***************** LOAD TGA *********************************




void init(void)
{
	//OpenGL flags
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	camera3D.lookAt(Vector3(-80,50,80),Vector3(0,50,0), Vector3(0,1,0));
	current_camera = &camera2D;

	e.loadTexture("muro.tga");
	s.loadTexture("suelo.tga");
	f.loadTexture("fuente.tga");
	est.loadTexture("gauss.tga");
	est.ini(500);
	
	polar.loadTexture("meteor.tga");
	polar.ini(1);
}

//Draw the grid
void drawGrid()
{
	float dist = 50.0f;
	int num_lines = 20;
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);
	glBegin( GL_LINES );
		for (int i = 0; i <= num_lines * 0.5; ++i)
		{
			float a = dist * num_lines * 0.5;
			float b = i * dist;

			if (i == num_lines * 0.5)
				glColor3f(1,0.25,0.25);
			else if (i%2)
				glColor3f(0.25,0.25,0.25);
			else
				glColor3f(0.5,0.5,0.5);


			glVertex3f(a,b,-a);
			glVertex3f(-a,b,-a);
			glVertex3f(a,-b,-a);
			glVertex3f(-a,-b,-a);
			glVertex3f(b,a,-a);
			glVertex3f(b,-a,-a);
			glVertex3f(-b,a,-a);
			glVertex3f(-b,-a,-a);
			glVertex3f(a,b,a);
			glVertex3f(-a,b,a);
			glVertex3f(a,-b,a);
			glVertex3f(-a,-b,a);
			glVertex3f(b,a,a);
			glVertex3f(b,-a,a);
			glVertex3f(-b,a,a);
			glVertex3f(-b,-a,a);


			glVertex3f(a,-a,b);
			glVertex3f(-a,-a,b);
			glVertex3f(a,-a,-b);
			glVertex3f(-a,-a,-b);

			glVertex3f(b,-a,a);
			glVertex3f(b,-a,-a);
			glVertex3f(-b,-a,a);
			glVertex3f(-b,-a,-a);

			glVertex3f(-a, a,b);
			glVertex3f(-a, -a,b);
			glVertex3f(-a, a,-b);
			glVertex3f(-a, -a,-b);
			glVertex3f(-a, b,a);
			glVertex3f(-a, b,-a);
			glVertex3f(-a, -b,a);
			glVertex3f(-a, -b,-a);
			glVertex3f(a, a,b);
			glVertex3f(a, -a,b);
			glVertex3f(a, a,-b);
			glVertex3f(a, -a,-b);
			glVertex3f(a, b,a);
			glVertex3f(a, b,-a);
			glVertex3f(a, -b,a);
			glVertex3f(a, -b,-a);
		}
	glEnd();
}

//what to do when the image has to be draw
void onDraw(void)
{
	current_camera->set();

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//drawGrid();

	glDisable( GL_DEPTH_TEST );
	est.Draw(current_camera);
	polar.DrawCenter(current_camera);
	glEnable( GL_DEPTH_TEST );
	
	s.Draw();
	e.Draw();
	f.Draw(20);

	//swap between front buffer and back buffer
	glutSwapBuffers();
}


//what to do on a window reshape
static void onReshape(int w, int h)
{
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	//Set the Camera
	camera2D.setOrthographic(w*-0.5,w*0.5,h*0.5,h*-0.5,-1000,1000);
	camera3D.setPerspective(70,w/(float)h,0.1,10000);

	//update the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( camera2D.projection_matrix.m );
}

//what to do on a key event
void onKeyEvent(unsigned char key, int x, int y)
{
	float speed = 10;
    switch (key) 
	{
		case 27: exit(0); break; //ESC
		case '1': current_camera = &camera2D; break;
		case '2': current_camera = &camera3D; break;
		case 'w':  camera3D.move(Vector3(0,0,speed)); break;
		case 's':  camera3D.move(Vector3(0,0,-speed)); break;
		case 'a':  camera3D.move(Vector3(speed,0,0)); break;
		case 'd':  camera3D.move(Vector3(-speed,0,0)); break;
		case 'q':  camera3D.rotate(-0.1, camera3D.up ); break;
		case 'e':  camera3D.rotate(0.1, camera3D.up ); break;
		case 'z':  camera3D.rotate(-0.1, camera3D.getLocalVector(Vector3(1,0,0)) ); break;
		case 'x':  camera3D.rotate(0.1, camera3D.getLocalVector(Vector3(1,0,0)) ); break;
		case ' ': init(); break;
	}
}

void onMouseEvent(int button, int state, int x, int y)
{
	//important to convert the coordinates from window coordinates to world coordinates
	int  width = glutGet(GLUT_WINDOW_WIDTH);
	int  height = glutGet(GLUT_WINDOW_HEIGHT);
	Vector3 last_point;
	last_point.x = x - width / 2;
	last_point.y = height - y - (height / 2);

	if ( button ==  GLUT_LEFT_BUTTON )
	{
		if ( state == GLUT_DOWN )
			pressed_points.push_back(last_point);
	}
	else if ( button ==  GLUT_RIGHT_BUTTON )
	{
		//find closer pressed point to the mouse pos and changes its pos to the new pos
		int id_closer = -1;
		float min_distance = 100000; //max value possible
		for (unsigned int i = 0; i < pressed_points.size(); ++i)
			if ( pressed_points[i].distance( last_point ) < min_distance)
			{
				id_closer = i;
				min_distance = pressed_points[i].distance( last_point );
			}
		if (id_closer != -1)
			pressed_points[id_closer] = last_point;
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
