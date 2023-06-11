#include "ElementBufferObjectClass.h"

ElementBufferObject::ElementBufferObject(GLuint* indicesToRender, GLsizeiptr size)
{
	//creates a empty buffer tied to the ID reference
	glGenBuffers(1, &ID);
	Bind();
	//inserts the render order data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicesToRender, GL_STATIC_DRAW);
}

void ElementBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBufferObject::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::Delete()
{
	glDeleteBuffers(1, &ID);
}