//libraries
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<chrono>
#include <time.h>
#include <cstdlib>

//custom openGl classes
#include "VertexArrayObjectClass.h"
#include "VertexBufferObjectClass.h"
#include "ElementBufferObjectClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"

//gameclasses
#include "Gameobject.h"

//rendering values
const int windowPixelWidth = 1000;
const int windowPixelHeight = 563;
const char* windowName = "OpenGLWindow";
const int speed = 3;

bool initGLFW();
GLFWwindow* initWindow();
void clearProgram(GLFWwindow* window);
void getInput(GLFWwindow* window, GLfloat* paddleOffsets, long& deltaT, GameObject& player1, GameObject& player2);
void draw(VertexArrayObject vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);

bool checkCollide(GameObject& r1, GameObject& r2, GLfloat* paddleOffsets);

void calculateBallOffset(GameObject& ball, GLfloat* paddleOffsets, long& deltaT, GameObject& player1, GameObject& player2);

GLfloat vertices[] = {
					   -0.03125f,	-0.05555f,	0.22f, 1.0f, 0.3f, //0 bottom left
						0.03125f,	-0.05555f,	1.6f, 0.66f, 0.9f, //1 top left
						0.03125f,	 0.05555f,	1.2f, 1.1f, 1.0f, //2 top right
					   -0.03125f,	 0.05555f,	1.77f, 1.0f, 0.1f, //3 bottom right
};

GLuint renderOrder[] = {
	0, 1, 3,//first paddle
	3, 1, 2
};

GLfloat scale[] = {//x y
	0.2f, 2.0f,	//paddles
	0.1f, 0.1f  //ball
};

int main()
{
	GameObject player1(-0.5f, 0.0f, 0.03125f*2.0f*0.2f, 0.05555f*2.0f*2.0f); //paddle1

	GameObject player2(0.5f, 0.0f, 0.03125f * 2.0f * 0.2f, 0.05555f * 2.0f * 2.0f); //paddle2

	GameObject player3(0.0f, 0.0f, 0.001, 0.001); //ball
	player3.velocityX = 0.5f;
	player3.velocityY = 0.1f;

	if (!initGLFW())
	{
		return -1;
	}

	GLFWwindow* window = initWindow();
	if (window == NULL)
	{
		return -1;
	}

	GLfloat paddleOffsets[] = {
		//paddle1
			player1.x,
			player1.y,
		//paddle2
			player2.x,
			player2.y,
		//ball
			player3.x,
			player3.y
			
	};

	Shader ShaderProgram("default.vert", "default.frag");

	VertexArrayObject VAO;
	VAO.Bind();

	VertexBufferObject positions(vertices, sizeof(vertices));
	VertexBufferObject offsets(paddleOffsets, sizeof(paddleOffsets));
	VertexBufferObject scales(scale, sizeof(scale));
	ElementBufferObject EBO(renderOrder, sizeof(renderOrder));

	//VertexBufferObject VBO, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset, GLuint divisor = 0);
	//tells OPENGL how to read the buffer, here we specify 2 sections of the buffer, the coordinates and the colour
	VAO.LinkVertexBuffferObject(positions, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0, 0);					//coord
	VAO.LinkVertexBuffferObject(positions, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)), 0);    //colour
	VAO.LinkVertexBuffferObject(offsets, 2, 2, GL_FLOAT, 2 * sizeof(float), (void*)(0 * sizeof(float)), 1);	//offsets
	VAO.LinkVertexBuffferObject(scales, 3, 2, GL_FLOAT, 2 * sizeof(float), (void*)(0 * sizeof(float)), 2);	//scales
	//Unbinds all Objects so that they aren't accidently modified
	VAO.Unbind();
	positions.Unbind();
	offsets.Unbind();
	scales.Unbind();
	EBO.Unbind();


	//set uniform values

	//loop
	long deltaT = 10000;
	while (!glfwWindowShouldClose(window))
	{
		
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		srand((unsigned)time(NULL));
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //an RGB argument
		glClear(GL_COLOR_BUFFER_BIT);

	
		ShaderProgram.Activate();

		getInput(window, paddleOffsets, deltaT, player1, player2);

		calculateBallOffset(player3, paddleOffsets, deltaT, player1,player2);

		offsets.UpdateData(sizeof(paddleOffsets) / sizeof(float), GL_FLOAT, 0, paddleOffsets);

		VAO.Bind();
		
		draw(VAO, GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		deltaT = std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count();
	}

	VAO.Delete();
	positions.Delete();
	offsets.Delete();
	ShaderProgram.Delete();
	EBO.Delete();

	clearProgram(window);

	return 0;
}

void draw(VertexArrayObject vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount) {
	glBindVertexArray(vao.ID);
	glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);
}

bool initGLFW()
{
	//error checking for openGL bootup
	if (!glfwInit())
	{
		std::cout << "Failed to initialise OpenGL. " << std::endl;
		glfwTerminate();
		return false;
	}
	//assign the version of OPENGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//chooses which funtions of OPENGL the program needs. CORE profile only uses modern OPENGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//configures stbi to load textures flipped so they render in correct orientation
	stbi_set_flip_vertically_on_load(true);
	return true;
}

GLFWwindow* initWindow()
{
	//opens a window, sets it's size and name
	GLFWwindow* window = glfwCreateWindow(windowPixelWidth, windowPixelHeight, windowName, NULL, NULL);
	//error checking for failed window launch
	if (window == NULL)
	{
		std::cout << "Failed to create the GLFW window named \"" << windowName << "\". " << std::endl;
		glfwTerminate();
		return NULL;
	}

	//sets the window to the current window so that program can modify it
	glfwMakeContextCurrent(window);

	//begins glad
	gladLoadGL();
	glViewport(0, 0, windowPixelWidth, windowPixelHeight);
	return window;
}

void clearProgram(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "OpenGL program closed." << std::endl;
}

bool checkCollide(GameObject& r1,GameObject& r2, GLfloat* paddleOffsets)
{
	if (r1.y <= r2.y + r2.colliderHeight / 2 && r1.y >= r2.y - r2.colliderHeight / 2)
	{
		//paddle collide with ball coming from left
		if ((r1.x + r1.colliderWidth / 2) <= (r2.x - r2.colliderWidth / 2) && (r2.x - r2.colliderWidth / 2)- (r1.x + r1.colliderWidth / 2) <= 0.01f)
		{
			r1.velocityX += 0.1;
			r1.x -= 0.01;
			paddleOffsets[4] -= 0.01;

			if (r1.y < r2.y + r2.colliderHeight /(2*5) && r1.y > r2.y - r2.colliderHeight / (2 * 5))
			{
				r1.velocityY = 0.0f;
				r1.velocityY += r2.velocityY;
			}
			else if (r1.y > r2.y + r2.colliderHeight / (2*5) )
			{
				r1.velocityY += 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - 0.5)));
				r1.velocityY += r2.velocityY;
			}
			else if (r1.y < r2.y - r2.colliderHeight / (2*5) )
			{
				r1.velocityY -= 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - 0.5)));;
				r1.velocityY += r2.velocityY;
			}

			return true;
		}
		if ((r1.x - r1.colliderWidth / 2) >= (r2.x + r2.colliderWidth / 2) && (r1.x - r1.colliderWidth / 2) - (r2.x + r2.colliderWidth / 2) <= 0.01f)
		{
			r1.velocityX -= 0.1;
			r1.x += 0.01;
			paddleOffsets[4] += 0.01;

			if (r1.y < r2.y + r2.colliderHeight / 5 && r1.y > r2.y - r2.colliderHeight / 5)
			{
				r1.velocityY = 0.0f;
				r1.velocityY += r2.velocityY;
			}
			else if (r1.y > r2.y + r2.colliderHeight / 5)
			{
				r1.velocityY += 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - 0.5)));;
				r1.velocityY += r2.velocityY;
			}
			else if (r1.y < r2.y - r2.colliderHeight / 5)
			{
				r1.velocityY -= 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - 0.5)));;
				r1.velocityY += r2.velocityY;
			}

			return true;
		}
	}
	
	return false;
}

void getInput(GLFWwindow* window, GLfloat* paddleOffsets, long& deltaT, GameObject& player1, GameObject& player2)
{
	player1.velocityY = 0.0f;
	player2.velocityY = 0.0f;
	GLfloat move = deltaT * pow(10, -6) * speed;
	//first paddle
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (paddleOffsets[1] > -0.88f)
		{ 
			player1.velocityY = -1;
			paddleOffsets[1] -= move;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (paddleOffsets[1] < 0.88f)
		{
			player1.velocityY = 1;
			paddleOffsets[1] += move;
		}
	}
	//second paddle
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (paddleOffsets[3] < 0.88f)
		{
			player2.velocityY = 1;
			paddleOffsets[3] += move;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (paddleOffsets[3] > -0.88f)
		{
			player2.velocityY = -1;
			paddleOffsets[3] -= move;
		}
	}
	player1.x = paddleOffsets[0];
	player1.y = paddleOffsets[1];
	player2.x = paddleOffsets[2];
	player2.y = paddleOffsets[3];
}

void calculateBallOffset(GameObject& ball, GLfloat* paddleOffsets, long& deltaT, GameObject& player1, GameObject& player2)
{
	ball.x = paddleOffsets[4];
	ball.y = paddleOffsets[5];

	//side walls
	if (paddleOffsets[4] >= 1.0f || paddleOffsets[4] <= -1.0f)
	{
		ball.velocityY = 0.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0 - 0.8))) * -1;
		ball.velocityX = ball.velocityX/(abs(ball.velocityX)) * 0.5;

		paddleOffsets[4] = 0.0f;
		paddleOffsets[5] = 0.0f;
	}
	//top-bot walls
	if (paddleOffsets[5] >= 1.0f || paddleOffsets[5] <= -1.0f)
	{
		ball.velocityY *= -1;
	}
	//ball collides with paddle 1
	if (checkCollide(ball, player1, paddleOffsets))
	{	
		ball.velocityX *= -1;
	}
	//ball collides with paddle 2
	if (checkCollide(ball, player2, paddleOffsets) == true)
	{
		ball.velocityX *= -1;
	}

	float deltaX = deltaT * pow(10, -6) * ball.velocityX;
	float deltaY = deltaT * pow(10, -6) * ball.velocityY;
	paddleOffsets[4] += deltaX;
	paddleOffsets[5] += deltaY;
}
