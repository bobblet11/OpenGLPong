#ifndef VERTEX_BUFFER_OBJECT_CLASS_H
#define VERTEX_BUFFER_OBJECT_CLASS_H

#include<glad/glad.h>

class VertexBufferObject
{
public:
	GLuint ID;
	VertexBufferObject(GLfloat* verticesToRender, GLsizeiptr size);

	void UpdateData(GLuint numberOfComponents, GLenum type, GLintptr offset, GLfloat* data);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // ! VERTEX_BUFFER_OBJECT_CLASS_H