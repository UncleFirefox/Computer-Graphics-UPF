//**************************************
#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

//Include del proyecto por defecto
#include "stdafx.h"

//include GLUT
#include <GL/glut.h>

//include libs
#include "stdlib.h"
#include "math.h"

//set the macros for the window size
#define SIZEX 500
#define SIZEY 500

const float PI = 3.1415926f;

//Returns a random number in the range [0.0f, 1.0f).  Every
//bit of the mantissa is randomized.
float rnd(void){
  //Generate a random number in the range [0.5f, 1.0f).
  unsigned int ret = 0x3F000000 | (0x7FFFFF & ((rand() << 8) ^ rand()));
  unsigned short coinFlips;

  //If the coin is tails, return the number, otherwise
  //divide the random number by two by decrementing the
  //exponent and keep going. The exponent starts at 63.
  //Each loop represents 15 random bits, a.k.a. 'coin flips'.
  #define RND_INNER_LOOP() \
    if( coinFlips & 1 ) break; \
    coinFlips >>= 1; \
    ret -= 0x800000
  for(;;){
    coinFlips = rand();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    //At this point, the exponent is 60, 45, 30, 15, or 0.
    //If the exponent is 0, then the number equals 0.0f.
    if( ! (ret & 0x3F800000) ) return 0.0f;
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
  }
  return *((float *)(&ret));
}

//Spiral Class
class Spiral
{
	float rotationangle, numrounds, radius;

public:
		Spiral (float, float, float);
		Spiral (float, float);
		void Rotate(float);
		void Draw();
		void DrawRotating(float);

};

Spiral::Spiral(float angle, float round, float rad)
{
	rotationangle = angle;
	numrounds = round;
	radius = rad;
}

Spiral::Spiral(float round, float rad)
{
	rotationangle = 0.0f;
	numrounds = round;
	radius = rad;
}

void Spiral::Rotate(float angle)
{
	if (rotationangle >= 2*PI)
	{
		rotationangle = angle;
	}
	else
	{
		rotationangle += angle;
	}
}

void Spiral::Draw()
{
	glBegin(GL_LINE_STRIP);
		
		float circlesamples = 100;

		for(float angle = rotationangle; angle < (2*PI*numrounds)+rotationangle; angle+=(1/circlesamples))
		{
			glColor3f (rnd(), rnd(), rnd());
			glVertex2d(radius*cos(angle)*((angle-rotationangle)/20),radius*sin(angle)*((angle-rotationangle)/20));
		}

	glEnd();
}

void Spiral::DrawRotating(float rotangle)
{
	Rotate(rotangle);
	Draw();
}

float angle_increment = 0.000000001f;
float translation_increment = 0.000000001f;

//initializing stuff
void init(void)
{
    glDisable( GL_DEPTH_TEST ); //triangles have two sides
	glDisable( GL_CULL_FACE ); //triangles have two sides
}

//this function updates the coordinates system
void onReshape(int w, int h)
{
	//range is the value from which the coordinates system goes, -range to +range
	float range = 1.0f;

	// Prevent a divide by zero
	if(h == 0) h = 1;

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

//process key strokes
void onKey(unsigned char key, int x, int y)
{
    switch (key) 
	{	//example of keyboard input
		case '0': glClearColor(0,0,0,0); break;
		case '1': glClearColor(0.2,0,0,0); break;
		case '2': glClearColor(0,0.2,0,0); break;
		case '3': glClearColor(0,0,0.2,0); break;

		case 27: exit(0); //ESC key
				 break;
	}
}

void drawCircle(void)
{
	float circlesamples = 100;
	float radius = 0.25f;
	glBegin(GL_LINE_STRIP);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0; angle < 2*PI; angle+=(1/circlesamples))
		{
			glVertex2d(radius*cos(angle),radius*sin(angle));
		}

	glEnd();

}

void drawSpiral(void)
{
	float circlesamples = 100;
	float radius = 0.25f;
	float numrotations = 5.0f;
	glBegin(GL_LINE_STRIP);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0.0f; angle < 2*PI*numrotations; angle+=(1/circlesamples))
		{
			glVertex2d(radius*cos(angle)*angle/10,radius*sin(angle)*angle/10);
			glColor3f (rnd(), rnd(), rnd());
		}

	glEnd();

}

void drawSpiralProgressively(void)
{
	float circlesamples = 100;
	float radius = 0.25f;
	float numrotations = 5.0f;
	
	glBegin(GL_LINE_STRIP);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0.0f; angle < (2*PI*numrotations) * angle_increment; angle+=(1/circlesamples))
		{
			glVertex2d(radius*cos(angle)*angle/10,radius*sin(angle)*angle/10);
			glColor3f (rnd(), rnd(), rnd());
		}

		if (!(angle_increment >= 1.0f))
		{
			angle_increment = angle_increment + 0.0002f;
		}

	glEnd();

}

void drawOval(void)
{
	float circlesamples = 100;
	float ovalwidth = 1.0f;
	float ovalheight = 2.0f;

	glBegin(GL_LINE_STRIP);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0.0f; angle < 2*PI; angle+=(1/circlesamples))
		{
			glVertex2d(ovalwidth/2*cos(angle),ovalheight/2*sin(angle));
		}

	glEnd();

}

void drawPoligon(float numpoints, float radius)
{
	float x,y;
	float incangle = (2*PI)/numpoints;

	glBegin(GL_LINE_STRIP);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0.0f; angle <= 2*PI; angle += incangle)
		{
			x = radius * cos(angle);
			y = radius*sin(angle);
			glColor3f (rnd(), rnd(), rnd());
			glVertex2d(x, y);
		}

	glEnd();

}

void drawBall(void)
{
	float circlesamples = 100;
	float radius = 0.10f;
	glBegin(GL_TRIANGLE_FAN);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		
		for(float angle = 0; angle < 2*PI; angle+=(1/circlesamples))
		{
			glVertex2d(-1.5f + translation_increment + radius*cos(angle),radius*sin(angle));
			glColor3f (1.0f, 0.0f, 0.0f);
		}

		if (!(translation_increment >= 1.5f))
		{
			translation_increment = translation_increment + 0.0002f;
		}

	glEnd();

}

void drawTriangle(void)
{
	//Render the triangle
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f ); //change vertex color
		glVertex2d( -0.5f, -0.5f );
		
		glColor3f(0.0f, 1.0f, 0.0f ); //change vertex color
		glVertex2d( 0.5f, -0.5f );
		
		glColor3f(0.0f, 0.0f, 1.0f ); //change vertex color
		glVertex2d( 0.0f, 0.5f );
	glEnd();
}

Spiral* DreamCast = new Spiral(4.0f,0.5f);

//Render one frame
void render(void)
{
	//Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DreamCast->DrawRotating(0.01f);

	/*if(translation_increment <= 1.5f)
	{
		drawBall();
	}
	else
	{
		drawSpiralProgressively();
	}*/

	//Swap front and back buffer
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	//init all OpenGL stuff
    glutInit(&argc, argv); //init GLUT
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE ); //init framebuffer properties
    glutInitWindowSize(SIZEX, SIZEY); //init window dimensions
    glutCreateWindow("Framework");	//create window

	//now that we have the window we can set the opengl state
    init(); 

	//set the callbacks
    glutReshapeFunc(onReshape); //when the window shape changes (resizing)
	glutKeyboardFunc(onKey);	  //when a key is pressed or released
    glutDisplayFunc(render);  //render a frame
	glutIdleFunc(render);		  //what to do when nothing happens

	//start the GLUT mainloop (this function will be executed in a loop)
    glutMainLoop();	  

	return 0;
}

