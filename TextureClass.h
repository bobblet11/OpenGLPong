#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<iostream>
#include <stb/stb_image.h>
#include<cerrno>
#include<string>
#include<glad/glad.h>

class Texture
{
public:
	GLuint ID;
	Texture(std::string textureFileName, GLint filterMethod, GLint S_repeatMethod, GLint T_repeatMethod);

	void Bind();
	void Unbind();
	void Delete();

};


#endif