#include "VertexArrayObjectClass.h"

VertexArrayObject::VertexArrayObject()
{
	//creates empty vertex array tied to ID reference
	glGenVertexArrays(1, &ID);
}

void VertexArrayObject::LinkVertexBuffferObject(VertexBufferObject VBO, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset, GLuint divisor)
{
	//         VERTEX 1                VERTEX2
	// COORDINATE    COLOR     COORDINATE    COLOR
	// X   Y   Z   R   G   B   X   Y   Z   R   G   B
	// |-----------------------|                        coordinates stride [ n * (24 bits)    ]
	//             |-----------------------|            colour stride [ n * (24 bits) + offset]
	// |-------|                                        colour offset [ 12 bits               ]

	//sets current buffer to the VBO with data in it
	VBO.Bind();
	//sets the vertex array object properties to start at the layout index and make 3 steps for each vertex
	glVertexAttribPointer(layout, numberOfComponents, type, GL_FALSE, stride, offset);
	//enables the vertex array object
	glEnableVertexAttribArray(layout);
	VBO.Unbind();

	if (divisor > 0)
	{
		glVertexAttribDivisor(layout, divisor);
	}
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(ID);
}

void VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}

void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &ID);
}