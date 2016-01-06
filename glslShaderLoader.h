#pragma once

#include"ogldev_util.h"
#include<gl\glew.h>
#include<gl\glut.h>


class glslShaderLoader{

protected:
	GLchar *vertexName ;
	GLchar *geometryName ;
	GLchar *fragmentName ;
	GLenum shaderType ;
	GLuint ShaderProgram ;


public:

	glslShaderLoader(){vertexName = 0; geometryName = 0; fragmentName =0; ShaderProgram = 0; shaderType = -1;}


	void loadShader(const char *vetexName_=NULL,const char *geometryName_=NULL,
		              const char *fragmentName_=NULL);


	void reload();

	void addShader(string s,GLenum id);

	void useShder();

	GLuint getUniform(const char *name);

	void DisUse();

	GLuint getProgram(){ return ShaderProgram; };

};