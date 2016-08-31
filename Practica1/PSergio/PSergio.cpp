/*
*********************************************
** UPF/ETIS - Infografia I, Curso 2010/2011
**
** Practica 1
** Sergio Leon Gaixas - NIA 100750
** Albert Rodríguez Franco - NIA 103117
*********************************************
*/
#ifdef WIN32 //if we are under windows...
	//we need to have this include
	#include <windows.h> 
#endif

//include GLUT
#include <GL/glut.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
#include "stdafx.h"

//set the macros for the window size
#define SIZEX 500
#define SIZEY 500


#define HITOBJ_N 0
#define PISTA_N 1
#define POLIGONO_N 2

#ifndef PI
	const double PI = 3.1415926;
#endif

//Definicion de funciones

float rnd(void);

class hitObj{
    public:
    float x,y,rad, R,G,B, angle, intR, intG, intB;
	float intC[2][64];
	float extC[2][64];
    int tip;
    hitObj(float _rad, float _R, float _G, float _B);
    bool inRadius(hitObj* o);
    float hitAngle(hitObj* o);
};

class ball : public hitObj {
    public:
    float v;
    float lx,ly;
    float fx,fy;
	float intC[2][16];
    ball(float _x, float _y, float _rad, float _R=0.0f, float _G=0.0f, float _B=0.8f, float _v=0.01, float _angle=0);
    void draw();
    void update();
    void revert();
    bool inRadius(hitObj* o);
	float ball::hitAngle(hitObj* o);
    void setAngle(float* _angle, int n);
    void actua(hitObj** o, int n);
};

class pista : public hitObj {
    public:
    pista(float _R=0.8f, float _G=0.4f, float _B=0.2f);
    bool inRadius(hitObj* o);
    void draw();
};

class poligono : public hitObj {
    public:
    float points_x[5], points_y[5];
    poligono(float _rad, float _R=0, float _G=0, float _B=0, float _angle=0);
    float hitAngle(hitObj* o);
    bool inRadius(hitObj* o);
    void draw();
    void update(unsigned char c);
};

//Spiral Class
class Spiral
{
	float rotationangle, numrounds, radius;

public:
		Spiral (float angle, float round, float rad);
		Spiral (float round, float rad);
		void Rotate(float);
		void Draw();
		void up();
		void down();
		void Update(float a = 0.1f);
};

class mundo {
	public:
	pista *p;
	poligono *pl;
	Spiral *s1, *s2;
	hitObj *ls[50];
	float dir;
	int n;
	ball **bl;
	mundo(int _n);
	void update();
	void draw();
	void Key(unsigned char c);
};

float min(float a, float b);

float max(float a, float b);

//Funciones


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

hitObj::hitObj(float _rad, float _R, float _G, float _B){
    x=0; y=0; rad=_rad; R=_R; G=_G; B=_B;
	tip = HITOBJ_N; angle = 0;
	intR = R/2.0f; intG = G/2.0f; intB = B/2.0f;
};

bool hitObj::inRadius(hitObj* o){
    return ( ( rad + o->rad ) >= ( sqrt( pow( o->x, 2 ) + pow( o->y, 2 ) ) ) );
};

float hitObj::hitAngle(hitObj* o){
	if(o->x == 0) {
		return 0 - o->angle;
	} else {
		float dch = atan( ( o->y ) / ( o->x ) );
		return ((float)PI + dch + dch - o->angle);
	}
};

ball::ball(float _x, float _y, float _rad, float _R, float _G, float _B, float _v, float _angle) : hitObj(_rad, _R, _B, _G){
    x=_x; y=_y; v=_v;
    float _angles[1];
    _angles[0] = _angle;
    this->setAngle(_angles,1);
    lx=_x;ly=_y;fx=_x;fy=_y;
	
	for (int i = 0; i<16; i++) {
		intC[0][i] = cos(i*2*PI/16)*_rad;
		intC[1][i] = sin(i*2*PI/16)*_rad;
	}
};
void ball::draw(){
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(intR, intG, intB ); 
		glVertex2f(x,y );
		for(int i = 0; i<16; i++) {
			glVertex2f(intC[0][i]+x,intC[1][i]+y );
		}
		glVertex2f(intC[0][0]+x,intC[1][0]+y );
	glEnd();
	glBegin(GL_LINE_STRIP);
		glColor3f(R, G, B ); 
		for(int i = 0; i<16; i++) {
			glVertex2f(intC[0][i]+x,intC[1][i]+y );
		}
			glVertex2f(intC[0][0]+x,intC[1][0]+y );
	glEnd();

	glBegin(GL_LINES);
		glColor4f(R, G, B, 0.7f ); 
		glVertex2f(x, y );
		glVertex2f(lx, ly );
		
		glColor4f(R, G, B, 0.3f ); 
		glVertex2f(lx,ly );
		glVertex2f(fx,fy );
	glEnd();
};
bool ball::inRadius(hitObj* o){
    return ( ( rad + o->rad+((ball*)o)->v ) >= ( sqrt( pow( x - o->x, 2 ) + pow( y - o->y, 2 ) ) ) );
};
float ball::hitAngle(hitObj* o){
	if(x - o->x == 0) {
		return - o->angle;
	} else {
		float dch = atan( ( y - o->y ) / ( x - o->x ) );
		return ((float)PI- o->angle + dch + dch);
	}
};
void ball::update(){
    x = x + v*cos(angle);
    y = y + v*sin(angle);
};
void ball::revert(){
    angle = angle + PI;
	if(angle>= 2*PI) {
		angle = angle - 2*PI;
	}
};
void ball::setAngle(float* _angle, int n){
    float ang;
    if( n = 1 ) {
        ang = _angle[0];
    } else {
        ang = 0;
        for(int i = 0; i<n; i++) {
            ang = ang + _angle[i];
        }
        ang = ang / n;
    }
    if( ang < 0 ){
        int temp = (int) ceil( ang / (2 * PI) );
        ang = ang + temp * 2 * PI;
    } else if( ang > ( 2 / PI ) ) {
        int temp = (int) floor( ang / (2 * PI) );
        ang = ang - temp * 2 * PI;
    }
    angle = ang;
};
void ball::actua(hitObj** o, int n){
    int cl = 0;
    float _angles[50];
    for(int j = 0; j<n;j++){
        if(o[j] != this) {
            switch(o[j]->tip) {
                case PISTA_N :
                     if(((pista*) o[j])->inRadius(this)) {
                        _angles[cl] = o[j]->hitAngle(this);
                        cl++;
                    }
                break;
                case POLIGONO_N :
                     if(((poligono*) o[j])->inRadius(this)) {
                        _angles[cl] = ((poligono*) o[j])->hitAngle(this);
                        cl++;
                    }
                break;
                case HITOBJ_N :
                default :
                    if(((ball*)o[j])->inRadius(this)) {
                        _angles[cl] = ((ball*)o[j])->hitAngle(this);
                        cl++;
                    }
                break;
            }

        }
    }
    if(cl>0){
		fx=lx;fy=ly;
		lx=x;ly=y;
        this->setAngle(_angles, cl);
    }
}

pista::pista(float _R, float _G, float _B) : hitObj(0.97, _R, _B, _G) {
	tip=PISTA_N;

	float x,y;
	for (int i = 0; i<64; i++) {
		x = cos(i*2*PI/64);
		y = sin(i*2*PI/64);
		intC[0][i] = x*0.97f;
		intC[1][i] = y*0.97f;
		extC[0][i] = x;
		extC[1][i] = y;
	}
};
void pista::draw(){
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(R, G, B ); 
		glVertex2d(0.0f,0.0f );
		for(int i = 0; i<64; i++) {
			glVertex2d(extC[0][i],extC[1][i] );
		}
		glVertex2d(extC[0][0],extC[1][0] );
	glEnd();	
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(intR, intG, intB ); 
		glVertex2d(0.0f,0.0f );
		for(int i = 0; i<64; i++) {
			glVertex2d(intC[0][i],intC[1][i] );
		}
		glVertex2d(intC[0][0],intC[1][0] );
	glEnd();
	
};
bool pista::inRadius(hitObj* o){
	
    return ( ( 0.97 - o->rad ) <= ( sqrt( pow( o->x, 2 ) + pow( o->y, 2 ) ) ) );
};


poligono::poligono(float _rad, float _R, float _G, float _B, float _angle) : hitObj(_rad, _R, _B, _G){
    angle=_angle;tip=POLIGONO_N;

	//cuad 1
    points_x[4] = points_x[0] = cos(angle) * _rad;
	points_y[4] = points_y[0] = sin(angle) * _rad;
	//cuad 2
    points_x[1] = cos(angle + PI/2) * _rad;
    points_y[1] = sin(angle + PI/2) * _rad;
	//cuad 3
    points_x[2] = cos(angle + PI) * _rad;
    points_y[2] = sin(angle + PI) * _rad;
	//cuad 4
    points_x[3] = cos(angle + 3*PI/2) * _rad;
    points_y[3] = sin(angle + 3*PI/2) * _rad;
};

bool poligono::inRadius(hitObj* o){
	float _rad = rad+o->rad;

	float tpoints_x[5], tpoints_y[5];
	//cuad 1
    tpoints_x[4] = tpoints_x[0] = cos(angle) * _rad;
	tpoints_y[4] = tpoints_y[0] = sin(angle) * _rad;
	//cuad 2
    tpoints_x[1] = cos(angle + PI/2) * _rad;
    tpoints_y[1] = sin(angle + PI/2) * _rad;
	//cuad 3
    tpoints_x[2] = cos(angle + PI) * _rad;
    tpoints_y[2] = sin(angle + PI) * _rad;
	//cuad 4
    tpoints_x[3] = cos(angle + 3*PI/2) * _rad;
    tpoints_y[3] = sin(angle + 3*PI/2) * _rad;


    float x0 = tpoints_x[0];
	float y0 = tpoints_y[0];
	float x1,y1,xinters;
	int counter = 0;
	for (int i = 1; i <= 4; i++) {
		x1 = tpoints_x[i];
		y1 = tpoints_y[i];
		if ((o->y ) > min(y0, y1)) {
			if ((o->y) <= max(y0, y1)) {
				if ((o->x) <= max(x0, x1)) {
					if (y0 != y1) {
						xinters = (o->y - y0) * (x1 - x0) / (y1 - y0) +x0;
						if (x0 == x1 || (o->x) <= xinters) {
							counter++;
						}
					}
				}
			}
		}
		x0 = x1;
		y0 = y1;
	}
	return (counter % 2 != 0);
};

float poligono::hitAngle(hitObj* o) {
	float dch;
	if(( x - o->x ) == 0) {
		dch = PI/2;
	} else {
		dch = atan( ( y - o->y ) / ( x - o->x ) );
	}
    if(dch<0) {
        dch = dch+PI;
    }
    if(0>o->y) {
        dch = dch+PI;
    }

    float temp = dch + dch - o->angle;
    float ant = angle - PI / 2;
    float act;

    for(int i = 0; i<=4;i++){
        act =  i * PI / 2 + angle;
        if(dch == act) {
            return PI + act + act - o->angle;
        } else if(dch < act) {
            return PI + act + ant - o->angle;
        }
        ant = act;
    }
    return  o->angle;
};
void poligono::draw() {
    glBegin(GL_QUADS);
		glColor3f(intR, intG, intB ); 
		for(int i = 0; i<4; i++) {
			glVertex2f(points_x[i],points_y[i]);
		}
	glEnd();	
    glBegin(GL_LINE_STRIP);
		glColor3f(R, G, B ); 
		for(int i = 0; i<=4; i++) {
			glVertex2f(points_x[i],points_y[i]);
		}
	glEnd();	
    glBegin(GL_POINTS);
		glColor3f(R, G, B ); 
		glVertex2f(0.0f,0.0f);
	glEnd();	
};
void poligono::update(unsigned char c) {
	if(c == '4') {
		angle = angle + 0.2;
		if(angle > PI/2) {
			angle = angle -PI/2;
		}
		//cuad 1
    points_x[4] = points_x[0] = cos(angle) * rad;
	points_y[4] = points_y[0] = sin(angle) * rad;
	//cuad 2
    points_x[1] = cos(angle + PI/2) * rad;
    points_y[1] = sin(angle + PI/2) * rad;
	//cuad 3
    points_x[2] = cos(angle + PI) * rad;
    points_y[2] = sin(angle + PI) * rad;
	//cuad 4
    points_x[3] = cos(angle + 3*PI/2) * rad;
    points_y[3] = sin(angle + 3*PI/2) * rad;
	}	
	if(c == '6') {
		angle = angle - 0.2;
		if(angle < 0) {
			angle = angle +PI/2;
		}
		//cuad 1
    points_x[4] = points_x[0] = cos(angle) * rad;
	points_y[4] = points_y[0] = sin(angle) * rad;
	//cuad 2
    points_x[1] = cos(angle + PI/2) * rad;
    points_y[1] = sin(angle + PI/2) * rad;
	//cuad 3
    points_x[2] = cos(angle + PI) * rad;
    points_y[2] = sin(angle + PI) * rad;
	//cuad 4
    points_x[3] = cos(angle + 3*PI/2) * rad;
    points_y[3] = sin(angle + 3*PI/2) * rad;
	}
}

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

void Spiral::Draw()
{
	glBegin(GL_LINE_STRIP);
		
		float circlesamples = 0.01f;
		float max = (2*PI*numrounds);
		float limit = radius/max;
		for(float angle = 0; angle < max; angle+=circlesamples)
		{
			glColor3f (rnd(), rnd(), rnd());
			glVertex2d(limit*cos(angle+rotationangle)*angle,limit*sin(angle+rotationangle)*angle);
		}

	glEnd();
}

void Spiral::Update(float a)
{
	rotationangle += a;
	if (rotationangle >= 2*PI)
	{
		rotationangle -= 2*PI;
	}
}

void Spiral::up()
{
	numrounds -= 0.1f;
	
	if (numrounds <= 0.5)
	{
		numrounds = 0.5;
	}
}
void Spiral::down()
{
	numrounds += 0.1f;
	
	if (numrounds >= 5)
	{
		numrounds = 5;
	}
}

mundo::mundo(int _n) {
	if(_n>48) {
		printf("Numero maximo de bolas 48\n");
		_n=48;
	}
	ls[0] = p = new pista();
	ls[1] = pl = new poligono(0.4f, 0.3f, 0.9f,0.9f);
	s1 = new Spiral(3.4f,0.97f);
	s2 = new Spiral(PI,3.4f,0.97f);
	n=_n;
	bl = (ball**) malloc(sizeof(ball*)*n);

	for(int i = 0; i<_n; i++ ) {
		bl[i] = new ball(cos(i*2*PI/n)/2,sin(i*2*PI/n)/2,0.03f, rnd(),rnd(),rnd(), 0.005f, 0.0f);
		ls[i+2] = bl[i];
	}
};
void mundo::draw(){
	glClearColor(0.2f, 0.0f, 0.0f,0.0f); 
	p->draw();
	pl->draw();
	s1->Draw();
	s2->Draw();
	
	for(int i = 0; i<n; i++ ) {
		bl[i]->draw();
	}
	
};
void mundo::update(){
	for(int i = 0; i<n; i++ ) {
		(bl[i])->actua(ls,n+2);
	}

	for(int i = 0; i<n; i++ ) {
		bl[i]->update();
	}
	s1->Update();
	s2->Update();
};

void mundo::Key(unsigned char c) {
	switch(c) {
		case 32:
		case 53:
			for(int i = 0; i<n; i++ ) {
				bl[i]->revert();
			}
			break;
		case 52:
		case 54:
			pl->update(c);
			break;
		case 50:
			s1->up();
			s2->up();
			break;
		case 56:
			s1->down();
			s2->down();
			break;
	}
}


float min(float a, float b) {
	if(a>b) {
		return b;
	} else {
		return a;
	}
};

float max(float a, float b) {
	if(a<b) {
		return b;
	} else {
		return a;
	}
};


mundo *m;

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
	m->Key(key);
    if (key == 27) 
	{	
		exit(0); //ESC key
	}
}

//Render one frame
void render(void)
{
	//Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m->draw();

	//Swap front and back buffer
	glutSwapBuffers();
}

void timer(int n) {
	m->update();
    glutTimerFunc(5, timer,0);
}

int main(int argc, char **argv)
{
	m = new mundo(10);

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
    glutTimerFunc(5, timer,0);
	//start the GLUT mainloop (this function will be executed in a loop)
    glutMainLoop();	  

	return 0;
}