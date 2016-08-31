#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <map>
#include "framework.h"

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#endif

#define CHECK_SHADER_VAR(a,b) if (a == -1) { std::cout << "Shader error: Var not found in shader \"" << vs_filename << "," << ps_filename<< "\" : " << b << std::endl; return; } 
//#define CHECK_SHADER_VAR(a,b) if (a == -1) return
//#define CHECK_SHADER_VAR(a,b) assert(a != -1 && "GLSL ShaderSet Error: Var not found")

class Shader
{
	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};	
	typedef std::map<const char*, int, ltstr> loctable;

public:
	Shader();
	virtual ~Shader();

	virtual void setFilenames(const std::string& vsf, const std::string& psf); //set but not compile
	virtual bool compile();
	virtual bool recompile();

	virtual bool load(const std::string& vsf, const std::string& psf);

	//internal functions
	virtual bool compileFromMemory(const std::string& vsm, const std::string& psm);
	virtual void release();
	virtual void enable();
	virtual void disable();

	static void init();
	static void disableShaders();

	//upload
	virtual void setFloat(const char* varname, const float& input) { setUniform1(varname, input); }
	virtual void setVector3(const char* varname, const Vector3& input) { setUniform3(varname, input.x, input.y, input.z); }
	virtual void setMatrix44(const char* varname, const float* m);

	virtual void setUniform1Array(const char* varname, const float* input, const int count) ;
	virtual void setUniform2Array(const char* varname, const float* input, const int count) ;
	virtual void setUniform3Array(const char* varname, const float* input, const int count) ;
	virtual void setUniform4Array(const char* varname, const float* input, const int count) ;

	virtual void setUniform1Array(const char* varname, const int* input, const int count) ;
	virtual void setUniform2Array(const char* varname, const int* input, const int count) ;
	virtual void setUniform3Array(const char* varname, const int* input, const int count) ;
	virtual void setUniform4Array(const char* varname, const int* input, const int count) ;

	virtual void setUniform1(const char* varname, const int input1) ;
	virtual void setUniform2(const char* varname, const int input1, const int input2) ;
	virtual void setUniform3(const char* varname, const int input1, const int input2, const int input3) ;
	virtual void setUniform3(const char* varname, const Vector3& input) { setUniform3(varname, input.x, input.y, input.z); }
	virtual void setUniform4(const char* varname, const int input1, const int input2, const int input3, const int input4) ;

	virtual void setUniform1(const char* varname, const float input) ;
	virtual void setUniform2(const char* varname, const float input1, const float input2) ;
	virtual void setUniform3(const char* varname, const float input1, const float input2, const float input3) ;
	virtual void setUniform4(const char* varname, const float input1, const float input2, const float input3, const float input4) ;

	virtual void setTexture(const char* varname, const unsigned int tex) ;

	virtual int getAttribLocation(const char* varname);
	virtual int getUniformLocation(const char* varname);

	std::string getInfoLog() const;
	bool hasInfoLog() const;

protected:

	bool readFile(const std::string& filename, std::string& content);
	bool compiled;

	std::string info_log;
	std::string vs_filename;
	std::string ps_filename;

	bool createVertexShaderObject(const std::string& shader);
	bool createFragmentShaderObject(const std::string& shader);
	bool createShaderObject(unsigned int type, GLhandleARB& handle, const std::string& shader);
	void saveInfoLog(GLhandleARB obj);

	bool validate();
	GLint getLocation(const char* varname, loctable* table);
	loctable locations;	

	GLhandleARB vs;
	GLhandleARB fs;
	GLhandleARB program;
	std::string log;
};

#endif