#include "shader.h"
#include <cassert>
#include <iostream>

Shader::Shader()
{
	compiled = false;
}

Shader::~Shader()
{
	release();
}

void Shader::init()
{
	static bool firsttime = true;
	
	if (firsttime)
		glewInit();
	firsttime = false;
}

void Shader::setFilenames(const std::string& vsf, const std::string& psf)
{
	vs_filename = vsf;
	ps_filename = psf;
}

bool Shader::load(const std::string& vsf, const std::string& psf)
{
	assert(	compiled == false );

	vs_filename = vsf;
	ps_filename = psf;
	
	printf("Vertex shader:\n%s\n", vsf.c_str());
	printf("Fragment shader:\n%s\n", psf.c_str());
	std::string vsm,psm;
	if (!readFile(vsf,vsm) || !readFile(psf,psm))
		return false;

	printf("Vertex shader from memory:\n%s\n", vsm.c_str());
	printf("Fragment shader from memory:\n%s\n", psm.c_str());

	if (!compileFromMemory(vsm,psm))
		return false;

	return true;
}

bool Shader::compile()
{
	assert(!compiled && "Shader already compiled" );
    return load(vs_filename,ps_filename);
}

bool Shader::recompile()
{
	release(); //remove old shader
    return load(vs_filename,ps_filename);
}

bool Shader::readFile(const std::string& filename, std::string& content)
{
	content.clear();
	FILE *fp;

	int count=0;

	fp = fopen(filename.c_str(),"rb");
	if (fp == NULL) 
	{
		printf("Shader::readFile: file not found %s\n",filename.c_str());
		return false;
	}

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content.resize(count);
	if (count > 0) 
	{
		count = fread(&content[0],sizeof(char),count,fp);
	}
	fclose(fp);

	return true;
}

std::string Shader::getInfoLog() const
{
	return info_log;
}

bool Shader::hasInfoLog() const
{
	return info_log.size() > 0; 
}

// ******************************************

bool Shader::compileFromMemory(const std::string& vsm, const std::string& psm)
{
	if (glCreateProgramObjectARB == 0)
	{
		std::cout << "Error: your graphics cards dont support shaders. Sorry." << std::endl;
		exit(0);
	}

	program = glCreateProgramObjectARB();
	assert (glGetError() == GL_NO_ERROR);

	if (!createVertexShaderObject(vsm))
	{
		printf("Vertex shader compilation failed\n");
		return false;
	}

	if (!createFragmentShaderObject(psm))
	{
		printf("Fragment shader compilation failed\n");
		return false;
	}

	glLinkProgramARB(program);
	assert (glGetError() == GL_NO_ERROR);

	GLint linked=0;
	glGetObjectParameterivARB(program,GL_OBJECT_LINK_STATUS_ARB,&linked);
	assert(glGetError() == GL_NO_ERROR);

	if (!linked)
	{
		saveInfoLog(program);
		release();
		return false;
	}

#ifdef _DEBUG
	validate();
#endif

	compiled = true;

	return true;
}

bool Shader::validate()
{
	glValidateProgramARB(program);
	assert ( glGetError() == GL_NO_ERROR );

	GLint validated = 0;
	glGetObjectParameterivARB(program,GL_OBJECT_VALIDATE_STATUS_ARB,&validated);
	assert(glGetError() == GL_NO_ERROR);
	
	if (!validated)
	{
		printf("Shader validation failed\n");
		saveInfoLog(program);
		return false;
	}

	return true;
}

bool Shader::createVertexShaderObject(const std::string& shader)
{
	return createShaderObject(GL_VERTEX_SHADER_ARB,vs,shader);
}

bool Shader::createFragmentShaderObject(const std::string& shader)
{
	return createShaderObject(GL_FRAGMENT_SHADER_ARB,fs,shader);
}

bool Shader::createShaderObject(unsigned int type, GLhandleARB& handle, const std::string& shader)
{
	handle = glCreateShaderObjectARB(type);
	assert (glGetError() == GL_NO_ERROR);

	const char* ptr = shader.c_str();
	glShaderSourceARB(handle, 1, &ptr, NULL);
	assert ( glGetError() == GL_NO_ERROR);
	
	glCompileShaderARB(handle);
	assert( glGetError() == GL_NO_ERROR);

	GLint compile=0;
	glGetObjectParameterivARB(handle,GL_OBJECT_COMPILE_STATUS_ARB,&compile);
	assert( glGetError() == GL_NO_ERROR);

	//we want to see the compile log if we are in debug (to check warnings)
	#ifndef DUMPSHADER
		if (!compile)
	#endif
	{
		saveInfoLog(handle);
		printf("Shader code:%s\n", shader.c_str());
		return false;
	}

	glAttachObjectARB(program,handle);
	assert( glGetError() == GL_NO_ERROR);

	return true;
}

void Shader::release()
{
	if (program && vs)
	{
		glDetachObjectARB(program,vs);
		assert (glGetError() == GL_NO_ERROR);
		glDeleteObjectARB(vs);
		assert (glGetError() == GL_NO_ERROR);
		vs = 0;
	}

	if (program && fs)
	{
		glDetachObjectARB(program,fs);
		assert (glGetError() == GL_NO_ERROR);
		glDeleteObjectARB(fs);
		assert (glGetError() == GL_NO_ERROR);
		fs = 0;
	}

	if (program)
	{
		glDeleteObjectARB(program);
		assert (glGetError() == GL_NO_ERROR);
		program = 0;
	}

	compiled = false;
}


void Shader::enable()
{
	glUseProgramObjectARB(program);
	assert (glGetError() == GL_NO_ERROR);
}


void Shader::disable()
{
	glUseProgramObjectARB(0);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::disableShaders()
{
	glUseProgramObjectARB(0);
	assert (glGetError() == GL_NO_ERROR);
}


void Shader::saveInfoLog(GLhandleARB obj)
{
	int len = 0;
	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
	assert(glGetError() == GL_NO_ERROR);

	if (len > 0)
	{
		char* ptr = new char[len+1];
		GLsizei written=0;
		glGetInfoLogARB(obj, len, &written, ptr);
		ptr[written-1]='\0';
		assert(glGetError() == GL_NO_ERROR);
		log.append(ptr);
		delete[] ptr;

		printf("LOG **********************************************\n%s\n",log.c_str());
	}
}

GLint Shader::getLocation(const char* varname,loctable* table)
{
	if(varname == 0 || table == 0)
		return 0;

	GLint loc = 0;
	loctable* locs = table;

	loctable::iterator cur = locs->find(varname);
	
	if(cur == locs->end()) //not found in the locations table
	{
		loc = glGetUniformLocationARB(program, varname);
		if (loc == -1)
		{
			return -1;
		}

		//insert the new value
		locs->insert(loctable::value_type(varname,loc));
	}
	else //found in the table
	{
		loc = (*cur).second;
	}
	return loc;
}

int Shader::getAttribLocation(const char* varname)
{
	int loc = glGetAttribLocationARB(program, varname);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);

	return loc;
}

int Shader::getUniformLocation(const char* varname)
{
	int loc = getLocation(varname, &locations);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);
	return loc;
}

void Shader::setTexture(const char* varname, unsigned int tex)
{
	setUniform1(varname,(int)tex);
}

void Shader::setUniform1(const char* varname, int input1)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1iARB(loc, input1);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform2(const char* varname, int input1, int input2)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2iARB(loc, input1, input2);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform3(const char* varname, int input1, int input2, int input3)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3iARB(loc, input1, input2, input3);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform4(const char* varname, const int input1, const int input2, const int input3, const int input4)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4iARB(loc, input1, input2, input3, input4);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform1Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform2Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform3Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform4Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform1(const char* varname, const float input1)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1fARB(loc, input1);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform2(const char* varname, const float input1, const float input2)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2fARB(loc, input1, input2);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform3(const char* varname, const float input1, const float input2, const float input3)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3fARB(loc, input1, input2, input3);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform4(const char* varname, const float input1, const float input2, const float input3, const float input4)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4fARB(loc, input1, input2, input3, input4);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform1Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform2Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform3Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setUniform4Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::setMatrix44(const char* varname, const float* m)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniformMatrix4fvARB(loc, 1, GL_FALSE, m);
	assert (glGetError() == GL_NO_ERROR);
}