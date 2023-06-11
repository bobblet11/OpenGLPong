#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include<string>
#include<glad/glad.h>

std::string GetFileContents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	void Activate();
	void Delete();
};


#endif // ! SHADER_CLASS_H

