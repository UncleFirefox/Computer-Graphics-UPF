#include "Textura.h"
#include <math.h>

TGAInfo* LoadTGA(const char* filename)
{
    GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    GLuint temp;
    GLuint type = GL_RGBA;

    FILE * file = fopen(filename, "rb");
    
    if (file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
        memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
        fread(header, 1, sizeof(header), file) != sizeof(header))
    {
        if (file == NULL)
            return NULL;
        else
        {
            fclose(file);
            return NULL;
        }
    }

	TGAInfo* tgainfo = new TGAInfo;
    
    tgainfo->width = header[1] * 256 + header[0];
    tgainfo->height = header[3] * 256 + header[2];
    
    if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
    {
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
    tgainfo->bpp = header[4];
    bytesPerPixel = tgainfo->bpp / 8;
    imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
    tgainfo->data = (GLubyte*)malloc(imageSize);
    
    if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
    {
        if (tgainfo->data != NULL)
            free(tgainfo->data);
            
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
    for (GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
    {
        temp = tgainfo->data[i];
        tgainfo->data[i] = tgainfo->data[i + 2];
        tgainfo->data[i + 2] = temp;
    }
    
    fclose(file);

	return tgainfo;
}

bool Texture::loadTexture(const char *filename)
{
	TGAInfo * t = LoadTGA(filename);
	texture_id = 0;	
	if(t == NULL)
		return false;


	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, t->width, t->height, GL_RGB, GL_UNSIGNED_BYTE, t->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;
}

void Texture::enable()
{
	glEnable( GL_TEXTURE_2D ); 
	glBindTexture( GL_TEXTURE_2D, texture_id );
}
void Texture::disable()
{
	glDisable( GL_TEXTURE_2D );
}

void Texture::DrawQuad(float offset, float size) 
{
	enable();
	glColor3f(1.0f,1.0f,1.0f);
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); //texture coordinates
		glVertex2f( offset, offset ); //vertex
		glTexCoord2f(0,10); //texture coordinates
		glVertex2f( offset,offset + size );
		glTexCoord2f(10,10); //texture coordinates
		glVertex2f( offset + size,offset + size);
		glTexCoord2f(10,0); //texture coordinates
		glVertex2f( offset + size,offset);
	glEnd();
	disable();
}

void Escenario::Draw() 
{
	enable();
	glColor3f(1.0f,1.0f,1.0f);
	
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( -100, 60, 200 );
		glTexCoord2f(0,2); glVertex3f( -100, 60, 220 );
		glTexCoord2f(20,2); glVertex3f( 100, 60, 220 );
		glTexCoord2f(20,0); glVertex3f( 100, 60, 200 );
	glEnd();
	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,0); glVertex3f( -100, 80, 200 );
		glTexCoord2f(0.5f,2); glVertex3f( -100, 80, 220 );
		glTexCoord2f(20.5f,2); glVertex3f( 100, 80, 220 );
		glTexCoord2f(20.5f,0); glVertex3f( 100, 80, 200 );
	glEnd();
	
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( -100, 60, 200 );
		glTexCoord2f(2,0); glVertex3f( -100, 60, 220 );
		glTexCoord2f(2,2); glVertex3f( -100, 80, 220 );
		glTexCoord2f(0,2); glVertex3f( -100, 80, 200 );
	glEnd();
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( 100, 60, 200 );
		glTexCoord2f(2,0); glVertex3f( 100, 60, 220 );
		glTexCoord2f(2,2); glVertex3f( 100, 80, 220 );
		glTexCoord2f(0,2); glVertex3f( 100, 80, 200 );
	glEnd();

	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,0); glVertex3f( -200, 0, 200 );
		glTexCoord2f(0.5f,6); glVertex3f( -200, 60, 200 );
		glTexCoord2f(40.5f,6); glVertex3f( 200, 60, 200 );
		glTexCoord2f(40.5f,0); glVertex3f( 200, 0, 200 );
	glEnd();
	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,0); glVertex3f( -200, 60, 200 );
		glTexCoord2f(0.5f,2); glVertex3f( -200, 80, 200 );
		glTexCoord2f(10.5f,2); glVertex3f( -100, 80, 200 );
		glTexCoord2f(10.5f,0); glVertex3f( -100, 60, 200 );
	glEnd();
	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,0); glVertex3f( 100, 60, 200 );
		glTexCoord2f(0.5f,2); glVertex3f( 100, 80, 200 );
		glTexCoord2f(10.5f,2); glVertex3f( 200, 80, 200 );
		glTexCoord2f(10.5f,0); glVertex3f( 200, 60, 200 );
	glEnd();


	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,8); glVertex3f( -200, 80, 200 );
		glTexCoord2f(0.5f,10); glVertex3f( -200, 100, 200 );
		glTexCoord2f(40.5f,10); glVertex3f( 200, 100, 200 );
		glTexCoord2f(40.5f,8); glVertex3f( 200, 80, 200 );
	glEnd();

	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( 200, 0, 200 );
		glTexCoord2f(0,10); glVertex3f( 200, 100, 200 );
		glTexCoord2f(40,10); glVertex3f( 200, 100, -200 );
		glTexCoord2f(40,0); glVertex3f( 200, 0, -200 );
	glEnd();

	glBegin( GL_QUADS );
		glTexCoord2f(0.5f,0); glVertex3f( 200, 0, -200 );
		glTexCoord2f(0.5f,10); glVertex3f( 200, 100, -200 );
		glTexCoord2f(40.5f,10); glVertex3f( -200, 100, -200 );
		glTexCoord2f(40.5f,0); glVertex3f( -200, 0, -200 );
	glEnd();

	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( -200, 0, -200 );
		glTexCoord2f(0,10); glVertex3f( -200, 100, -200 );
		glTexCoord2f(40,10); glVertex3f( -200, 100, 200 );
		glTexCoord2f(40,0); glVertex3f( -200, 0, 200 );
	glEnd();

	disable();
}
void Suelo::Draw() 
{
	enable();
	glColor3f(1.0f,1.0f,1.0f);

	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( -200, 0, -200 );
		glTexCoord2f(0,40); glVertex3f( -200, 0, 200 );
		glTexCoord2f(40,40); glVertex3f( 200, 0, 200 );
		glTexCoord2f(40,0); glVertex3f( 200, 0, -200 );
	glEnd();
	
	glBegin( GL_QUADS );
		glTexCoord2f(0,20); glVertex3f( -500, 100, 200 );
		glTexCoord2f(0,50); glVertex3f( -500, 100, 500 );
		glTexCoord2f(100,50); glVertex3f( 500, 100, 500 );
		glTexCoord2f(100,20); glVertex3f( 500, 100, 200 );
	glEnd();
	glBegin( GL_QUADS );
		glTexCoord2f(0,20); glVertex3f( -500, 100, -200 );
		glTexCoord2f(0,50); glVertex3f( -500, 100, -500 );
		glTexCoord2f(100,50); glVertex3f( 500, 100, -500 );
		glTexCoord2f(100,20); glVertex3f( 500, 100, -200 );
	glEnd();
	
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( 200, 100, -200 );
		glTexCoord2f(0,40); glVertex3f( 200, 100, 200 );
		glTexCoord2f(30,40); glVertex3f( 500, 100, 200 );
		glTexCoord2f(30,0); glVertex3f( 500, 100, -200 );
	glEnd();
	
	glBegin( GL_QUADS );
		glTexCoord2f(0,0); glVertex3f( -200, 100, -200 );
		glTexCoord2f(0,40); glVertex3f( -200, 100, 200 );
		glTexCoord2f(30,40); glVertex3f( -500, 100, 200 );
		glTexCoord2f(30,0); glVertex3f( -500, 100, -200 );
	glEnd();

	disable();
}
void Fuente::Draw(float size)
{
	enable();
	double c = 0.261799387799149;
	glColor3f(1.0f,1.0f,1.0f);
	for (int i = 0; i <= 24; i++) {
		glBegin( GL_QUADS );
			glTexCoord2f(0,0); glVertex3f( size*cos(c*(double)i), 0, size*sin(c*(double)i) );
			glTexCoord2f(0,1); glVertex3f( size*cos(c*(double)i), 40, size*sin(c*(double)i) );
			glTexCoord2f(1,1); glVertex3f( size*cos(c*(double)(i+1)), 40, size*sin(c*(double)(i+1)) );
			glTexCoord2f(1,0); glVertex3f( size*cos(c*(double)(i+1)), 0, size*sin(c*(double)(i+1)) );
		glEnd();
	}
	disable();
}

void Estrellas::Draw(Camera *current_camera)
{
	Vector3 top, right, side,upRight, upLeft, downRight, downLeft;

	Vector3 front = current_camera->center - current_camera->eye;
	front.normalize();

	current_camera->up.normalize();

	right = front.cross(current_camera->up);
	//right.normalize();

	side = front.cross(current_camera->up);
	//side.normalize();

	top = side.cross(front);
	//top.normalize();

	upLeft = top - side;
	upRight = top + side;
	downLeft = Vector3(-top.x,-top.y,-top.z) + Vector3(-side.x,-side.y,-side.z);
	downRight = Vector3(-top.x,-top.y,-top.z) + side;

	this->enable();

	glEnable(GL_BLEND);

	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glColor3f(1.0f,1.0f,1.0f);

	glBegin( GL_QUADS );

	for (int i = 0; i < this->estrellas.size(); i++)
	{
		glTexCoord2f(0,0); glVertex3f( this->estrellas[i].x + (10.0f*upLeft.x), this->estrellas[i].y + (10.0f*upLeft.y), this->estrellas[i].z + (10.0f*upLeft.z));
		glTexCoord2f(0,1); glVertex3f( this->estrellas[i].x + (10.0f*upRight.x), this->estrellas[i].y + (10.0f*upRight.y), this->estrellas[i].z + (10.0f*upRight.z));
		glTexCoord2f(1,1); glVertex3f( this->estrellas[i].x + (10.0f*downRight.x), this->estrellas[i].y + (10.0f*downRight.y), this->estrellas[i].z + (10.0f*downRight.z));
		glTexCoord2f(1,0); glVertex3f( this->estrellas[i].x + (10.0f*downLeft.x), this->estrellas[i].y + (10.0f*downLeft.y), this->estrellas[i].z + (10.0f*downLeft.z));
	}


	glEnd();

	glDisable(GL_BLEND);

	this->disable();
}

void Estrellas::DrawCenter(Camera *current_camera)
{
	Vector3 top, right, side,upRight, upLeft, downRight, downLeft;

	Vector3 front = current_camera->center - current_camera->eye;
	front.normalize();

	current_camera->up.normalize();

	right = front.cross(current_camera->up);
	//right.normalize();

	side = front.cross(current_camera->up);
	//side.normalize();

	top = side.cross(front);
	//top.normalize();

	upLeft = top - side;
	upRight = top + side;
	downLeft = Vector3(-top.x,-top.y,-top.z) + Vector3(-side.x,-side.y,-side.z);
	downRight = Vector3(-top.x,-top.y,-top.z) + side;

	this->enable();

	glEnable(GL_BLEND);

	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glColor3f(1.0f,1.0f,1.0f);

	glBegin( GL_QUADS );

	for (int i = 0; i < this->estrellas.size(); i++)
	{
		glTexCoord2f(0,0); glVertex3f( (50.0f*upLeft.x), this->estrellas[i].y + (50.0f*upLeft.y), (50.0f*upLeft.z));
		glTexCoord2f(0,1); glVertex3f( (50.0f*upRight.x), this->estrellas[i].y + (50.0f*upRight.y), (50.0f*upRight.z));
		glTexCoord2f(1,1); glVertex3f( (50.0f*downRight.x), this->estrellas[i].y + (50.0f*downRight.y), (50.0f*downRight.z));
		glTexCoord2f(1,0); glVertex3f( (50.0f*downLeft.x), this->estrellas[i].y + (50.0f*downLeft.y), (50.0f*downLeft.z));
	}

	glEnd();

	glDisable(GL_BLEND);

	this->disable();
}

void Estrellas::ini(int numestrellas)
{
	for (int i = 0; i < numestrellas; i++)
	{
		this->estrellas.push_back(Vector3(rand()%1400 - 700, (rand()%100)+300, rand()%1400 - 700));
	}
}