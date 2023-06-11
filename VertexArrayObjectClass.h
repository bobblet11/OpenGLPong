#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include<glad/glad.h>
#include"VertexBufferObjectClass.h"

class VertexArrayObject
{
public:
	GLuint ID;
	VertexArrayObject();

	void LinkVertexBuffferObject(VertexBufferObject VBO, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VERTEX_ARRAY_OBJECT_H

