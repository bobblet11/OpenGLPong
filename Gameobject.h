#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<GLFW/glfw3.h>
#include<glad/glad.h>

class GameObject {
	public:
		GameObject(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			deltaX = 0.0f;
			deltaY = 0.0f;
		}

		void updatePosition()
		{
			x += deltaX;
			y += deltaY;
		}
		
		GLfloat x, y;
		GLfloat deltaX, deltaY;
		GLfloat width, height;
};




#endif