// Practica 4 Infografia I by Javi Agenjo. 2008

//includes
#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include "framework.h"
#include "mesh.h"
#include "camera.h"

#ifdef _WIN32
	#include "cSound.h"
#endif

#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//The model matrix
Mesh* object[3];
Matrix44 model[6];

Camera camera[2];

int ancho, alto;
float currentrandompos = 0;
int i = 0;

#ifdef _WIN32

	cSound SFXPlayer;

#endif

// *********************************

void init(void)
{
	//OpenGL flags
	glDisable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	//Set the Cameras
	camera[0].lookAt( Vector3(0,50,+200), Vector3(0,0,0), Vector3(0,1,0) );
	camera[0].setProperties( 45, WINDOW_WIDTH / ((float)WINDOW_HEIGHT / 2), 1, 1000 );
	
	camera[1].lookAt( Vector3(0,50,-200), Vector3(0,0,0), Vector3(0,1,0) );
	camera[1].setProperties( 45, WINDOW_WIDTH / ((float)WINDOW_HEIGHT / 2), 1, 1000 );

	//update the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( camera[0].projection_matrix.m );

	//create and object in the scene
	object[0] = new Mesh();
	object[1] = new Mesh();
	object[2] = new Mesh();
	//object[0]->createCube(50);
	//object[0]->createOctaedro(50);
	object[0]->createTriforce(25);
	object[1]->createOctaedro(50);
	object[2]->createPlatform(150);

	
	model[0].setTranslation(0,10,0);
	model[1].setTranslation(100,0,100);
	model[2].setTranslation(-100,0,-100);
	model[3].setTranslation(100,0,-100);
	model[4].setTranslation(-100,0,100);
	model[5].setIdentity();
}

//Draw the grid
void drawGrid()
{
	float dist = 100.0f;
	int num_lines = 20;
	glLineWidth(1);
	glBegin( GL_LINES );
		for (int i = 0; i < num_lines * 0.5; ++i)
		{
			if (i == 0)	glColor3f(1.0,0.0,0.0);
			else glColor3f(0.5,0.5,0.5);
			glVertex3f(dist * num_lines * 0.5,0.0f, i * dist);
			glVertex3f(dist * num_lines * -0.5,0.0f, i * dist);
			glVertex3f(dist * num_lines * 0.5,0.0f, i * -dist);
			glVertex3f(dist * num_lines * -0.5,0.0f, i * -dist);

			if (i == 0)	glColor3f(0.0,0.0,1.0);
			else glColor3f(0.5,0.5,0.5);
			glVertex3f(i * dist,0.0f, dist * num_lines * 0.5);
			glVertex3f(i * dist,0.0f, dist * num_lines * -0.5);
			glVertex3f(i * -dist,0.0f, dist * num_lines * 0.5);
			glVertex3f(i * -dist,0.0f, dist * num_lines * -0.5);
		}
	glEnd();
}

void loadMatrices(const Matrix44& model, const Matrix44& view)
{
	//We upload the matrices into opengl
	glMatrixMode( GL_MODELVIEW );
	Matrix44 modelview;
	modelview = model * view;
	glLoadMatrixf( modelview.m );
}

//what to do when the image has to be draw
void onDraw(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//We need an identity matrix as a model for the grid
	Matrix44 m; m.setIdentity();

	//Vista1
	glViewport(0,alto/2,ancho, alto/2);
	//We load the MODELVIEW into opengl
	loadMatrices(m,camera[0].view_matrix);
	drawGrid();

	//We load the MODELVIEW into opengl

	loadMatrices(model[0], camera[0].view_matrix);
	object[0]->renderTriforce();
	
	loadMatrices(model[1], camera[0].view_matrix);
	object[1]->render();

	loadMatrices(model[2], camera[0].view_matrix);
	object[1]->render();

	loadMatrices(model[3], camera[0].view_matrix);
	object[1]->render();

	loadMatrices(model[4], camera[0].view_matrix);
	object[1]->render();

	loadMatrices(model[5], camera[0].view_matrix);
	object[2]->render();

	
	//Vista2
	glViewport(0,0,ancho, alto/2);

	//We load the MODELVIEW into opengl
	loadMatrices(m,camera[1].view_matrix);
	drawGrid();

	//We load the MODELVIEW into opengl

	loadMatrices(model[0], camera[1].view_matrix);
	object[0]->renderTriforce();
	
	loadMatrices(model[1], camera[1].view_matrix);
	object[1]->render();

	loadMatrices(model[2], camera[1].view_matrix);
	object[1]->render();

	loadMatrices(model[3], camera[1].view_matrix);
	object[1]->render();

	loadMatrices(model[4], camera[1].view_matrix);
	object[1]->render();

	loadMatrices(model[5], camera[1].view_matrix);
	object[2]->render();
	
	//swap between front buffer and back buffer
	glutSwapBuffers();

	//After all we rotate the model of the object just to give some animation
	model[0].rotate(0.005,Vector3(0,1,0));
}

//what to do on a window reshape
static void onReshape(int w, int h)
{
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);
	ancho = w; alto = h;

	
	//Set the Cameras
	camera[0].setProperties( 45, w / ((float)h / 2), 1, 1000 );
	camera[1].setProperties( 45, w / ((float)h / 2), 1, 1000 );
	
	camera[0].updateProjectionMatrix();
	camera[1].updateProjectionMatrix();

	//update the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( camera[0].projection_matrix.m );
}

//what to do on a key event
void onKeyEvent(unsigned char key, int x, int y)
{
	switch (key) 
	{
		//case ' ': {init(); break;}

		case 27: exit(0); break; //ESC

		case 32: //Space Key 
		{
			if (i == 0){i = 1;}
			else{i = 0;}
			break;
		}

		case 'z':
			camera[i].zoomCameraLookingCenter(2.0f);
			break;
		case 'x':
			camera[i].zoomCameraLookingCenter(-2.0f);
			break;
		
		case 'c':
			
			float randompos;

			randompos = rand()%5;

			while (randompos == currentrandompos)
			{
				randompos = rand()%5;
			}

			currentrandompos = randompos;

			Vector3 newlook = model[(int)currentrandompos] * Vector3(0,0,0);
			camera[i].changeCameraLook(newlook);

			#ifdef _WIN32

				SFXPlayer.PlaySoundFile("changeview.wav");
			
			#endif

			break;

	}
}

//what to do on a key event
void onSpecialEvent(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_RIGHT:
			camera[i].rotateCameraLookingCenterY(1.0f);
			break;

		case GLUT_KEY_LEFT:
			camera[i].rotateCameraLookingCenterY(-1.0f);
			break;

		case GLUT_KEY_DOWN:
			camera[i].rotateCameraLookingCenterX(1.0f);
			break;

		case GLUT_KEY_UP:
			camera[i].rotateCameraLookingCenterX(-1.0f);
			break;
	}
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("My application");

    init();

	#ifdef _WIN32

		cSound MidiPlayer;
		MidiPlayer.PlaySoundFile("ocarinaopening.mid");

	#endif


    glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyEvent);
	glutSpecialFunc(onSpecialEvent);
    glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
    glutMainLoop();

	return 0;
}
