#include "VertexBufferObjectClass.h"

VertexBufferObject::VertexBufferObject(GLfloat* verticesToRender, GLsizeiptr size)
{
	//generates empty buffer in memory tied to the reference ID
	glGenBuffers(1, &ID);
	//sets the buffer as current buffer
	Bind();
	//inserts all the vertex data into the buffer
	glBufferData(GL_ARRAY_BUFFER, size, verticesToRender, GL_STATIC_DRAW);
}

void VertexBufferObject::Bind()
{
	//sets as current buffer
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBufferObject::Unbind()
{
	//resets the current buffer to NULL
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete()
{
	//deletes buffer from memory
	glDeleteBuffers (1, &ID);
}

void VertexBufferObject::UpdateData(GLuint numberOfComponents, GLenum type, GLintptr offset, GLfloat* data)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, (numberOfComponents * sizeof(type)), data);
	Unbind();
}