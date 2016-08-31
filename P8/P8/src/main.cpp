// Practica 9 Infografia I by Javi Agenjo. 2008

#include "shader.h"
#include "framework.h"
#include "mesh.h"
#include "camera.h"

#include <iostream> //to output

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//The model matrix
std::vector<Vector3> pressed_points;

Camera camera;
Shader* shader = NULL;
Matrix44 model;
Shader shader_models[2];

//Parameters to change with buttons
Vector3 light_position = Vector3(-100,200,0);
int shader_model = 0;
int mesh_model = 0;

//Ambient factors
Vector3 light_ambient = Vector3(0.1f, 0.1f, 0.1f);
Vector3 material_ambient = Vector3(252.0f/255.0f, 194.0f/255.0f, 0.0f/255.0f);

//Diffuse factors
Vector3 light_diffuse = Vector3(1,1,1);
Vector3 material_diffuse = Vector3(252.0f/255.0f, 194.0f/255.0f, 0.0f/255.0f);

//Specular factors
Vector3 light_specular = Vector3(1,1,1);
Vector3 material_specular = Vector3(252.0f/255.0f, 194.0f/255.0f, 0.0f/255.0f);
float alpha = 20.0f;

long last_time = 0;

// *********************************

void init(void)
{
	//init the shader library, do this only once
	Shader::init();

	//Create a shader from two files
	if ( shader_models[0].load("shaders/gouraud_model.vs","shaders/gouraud_model.ps") == NULL) //this renders using a simplified goureud equation
		exit(0);
	std::cout << "Gouraud loaded and compiled without errors\n*****************************************" << std::endl;

	if ( shader_models[1].load("shaders/phong_model.vs","shaders/phong_model.ps") == NULL) //this renders using a simplified goureud equation
		exit(0);
	std::cout << "Phong loaded and compiled without errors\n*****************************************" << std::endl;

	//Default gouraud
	shader = &shader_models[0];

	//OpenGL flags
	glDisable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	//camera settings
	camera.lookAt(Vector3(0,250,250),Vector3(0,0,0), Vector3(0,1,0));
	camera.setPerspective(70,1,0.1,10000);

	model.setIdentity();

	last_time = getTime();
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
	//time between frames
	//this is helpful to avoid different speeds in different computers
	long current_time = getTime();
	double elapsed_time = (current_time - last_time) * 0.001; //in seconds
	last_time = current_time;

	camera.set();

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrid();

	//render dot light
	glPointSize(5);
	glColor3f(light_diffuse.x, light_diffuse.y, light_diffuse.z);
	glBegin( GL_POINTS );
		glVertex3f( light_position.x,light_position.y,light_position.z);
	glEnd();

	//before painting the object, we enable the shader
	//we need to have the shader enabled to upload vars
	shader->enable();
	
	//Positional vars (eye and light positions)
	if (shader->getUniformLocation("light_position")!= -1)
	{
		shader->setVector3("light_position", light_position);
	}

	if(shader->getUniformLocation("eye_position") != -1)
	{
		shader->setVector3("eye_position",camera.eye);
	}


	//Ambient vars
	if(shader->getUniformLocation("light_ambient") != -1)
	{
		shader->setVector3("light_ambient",light_ambient);
	}
	if(shader->getUniformLocation("material_ambient") != -1)
	{
		shader->setVector3("material_ambient",material_ambient);
	}

	//Diffuse vars
	if(shader->getUniformLocation("light_diffuse") != -1)
	{
		shader->setVector3("light_diffuse",light_diffuse);
	}
	if(shader->getUniformLocation("material_diffuse") != -1)
	{
		shader->setVector3("material_diffuse",material_diffuse);
	}

	//Specular vars
	if(shader->getUniformLocation("light_specular") != -1)
	{
		shader->setVector3("light_specular",light_specular);
	}
	if(shader->getUniformLocation("material_specular") != -1)
	{
		shader->setVector3("material_specular",material_specular);
	}
	if(shader->getUniformLocation("alpha") != -1)
	{
		shader->setFloat("alpha",alpha);
	}

	//the model view projection
	if (shader->getUniformLocation("mvp") != -1) //check if the shader needs this var
		shader->setMatrix44("mvp", (model * camera.view_matrix * camera.projection_matrix).m ); //upload the mvp to the shader

	//the regular model of the object
	if (shader->getUniformLocation("model") != -1)
		shader->setMatrix44("model", model.m );

	//to rotate the normals we need the model without the translation
	if (shader->getUniformLocation("normal_model") != -1)
		shader->setMatrix44("normal_model", model.getRotationMatrix().m );

	//render sphere

	switch(mesh_model)
	{
		case 0:
		{
			glutSolidSphere(100,15,15);
			break;
		}
		case 1:
		{
			glutSolidCone(100,100,10,10);
			break;
		}
		case 2:
		{
			glutSolidCube(100);
			break;
		}
		case 3:
		{
			glutSolidTorus(30,60,15,15);
			break;
		}
		case 4:
		{
			glutSolidTeapot(30.0f);
			break;
		}
	}

	//disable the shader or everything will be painted using the shader
	shader->disable();

	//swap between front buffer and back buffer
	glutSwapBuffers();

	//animation
	//rotate the main object using the amount of elapsed time since last time
	model.rotate( elapsed_time, Vector3(1,1,0) );
	model.orthonormalize(); //solve a precission problem that can deform the model matrix
}


//what to do on a window reshape
static void onReshape(int w, int h)
{
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	//Set the Camera
	camera.aspect = w/(float)h;
	camera.updateProjectionMatrix();
}

//what to do on a key event
void onKeyEvent(unsigned char key, int x, int y)
{
	float speed = 10;
    switch (key) 
	{
		case 27: exit(0); break; //ESC
		case 'w':  camera.move(Vector3(0,0,speed)); break;
		case 's':  camera.move(Vector3(0,0,-speed)); break;
		case 'a':  camera.move(Vector3(speed,0,0)); break;
		case 'd':  camera.move(Vector3(-speed,0,0)); break;
		case 'q':  camera.rotate(-0.1, camera.up ); break;
		case 'e':  camera.rotate(0.1, camera.up ); break;
		case 'z':  camera.rotate(-0.1, camera.getLocalVector(Vector3(1,0,0)) ); break;
		case 'x':  camera.rotate(0.1, camera.getLocalVector(Vector3(1,0,0)) ); break;
		case 'o': light_position.x -= 10; break;
		case 'p': light_position.x += 10; break;
		case ' ': 
		{
			if (shader_model == 0)
			{
				shader = &shader_models[1];
				shader_model = 1;
			}
			else
			{
				shader = &shader_models[0];
				shader_model = 0;
			}
			break;
		}

		case '0':
		{
			mesh_model = 0;
			break;
		}

		case '1':
		{
			mesh_model = 1;
			break;
		}
		case '2':
		{
			mesh_model = 2;
			break;
		}
		case '3':
		{
			mesh_model = 3;
			break;
		}
		case '4':
		{
			mesh_model = 4;
			break;
		}
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
		//if ( state == GLUT_DOWN )
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
