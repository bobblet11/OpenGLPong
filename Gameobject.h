#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<GLFW/glfw3.h>
#include<glad/glad.h>

class GameObject {
	public:
		GameObject(GLfloat x, GLfloat y, float colliderWidth, float colliderHeight)
		{
			this->x = x;
			this->y = y;
			this->colliderHeight = colliderHeight;
			this->colliderWidth = colliderWidth;
		}

		GLfloat x, y;

		float colliderHeight, colliderWidth;

		float velocityX = 0.0f, velocityY =0.0f;
};




#endif