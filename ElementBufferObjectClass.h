#ifndef ELEMENT_BUFFER_OBJECT_CLASS_H
#define ELEMENT_BUFFER_OBJECT_CLASS_H

#include<glad/glad.h>

class ElementBufferObject
{
public:
	GLuint ID;
	ElementBufferObject(GLuint* indicesToRender, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // ! ELEMENT_BUFFER_OBJECT_CLASS_H
