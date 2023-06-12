//libraries
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//custom openGl classes
#include "VertexArrayObjectClass.h"
#include "VertexBufferObjectClass.h"
#include "ElementBufferObjectClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"

//gameclasses
#include "Gameobject.h"

//rendering values
const int windowPixelWidth = 800;
const int windowPixelHeight = 800;
const char* windowName = "OpenGLWindow";

bool initGLFW();
GLFWwindow* initWindow();
void clearProgram(GLFWwindow* window);
void getInput(GLFWwindow* window, GLfloat* offset);

int main()
{
	if (!initGLFW())
	{
		return -1;
	}

	GLFWwindow* window = initWindow();
	if (window == NULL)
	{
		return -1;
	}

	GLfloat vertices[] = {
						-0.5f, -0.5f,		0.22f, 1.0f, 0.3f, //0 bottom left
						-0.5f,  0.5f,		1.6f, 0.66f, 0.9f, //1 top left
						 0.5f,  0.5f,		1.2f, 1.1f, 1.0f, //2 top right
						 0.5f, -0.5f,		1.77f, 1.0f, 0.1f, //3 bottom right
						};

	GLuint renderOrder[] = {
		0, 1, 3,
		3, 1, 2
	};
			
	GameObject player(0.0f, 0.0f, 1.0f, 1.0f);

	GLfloat offset[] = {
						player.x,
						player.y
						};

	Shader ShaderProgram("default.vert", "default.frag");

	VertexArrayObject VAO;
	VAO.Bind();

	VertexBufferObject positions(vertices, sizeof(vertices));
	VertexBufferObject offsets(offset, sizeof(offset));

	ElementBufferObject EBO(renderOrder, sizeof(renderOrder));
	
	//tells OPENGL how to read the buffer, here we specify 2 sections of the buffer, the coordinates and the colour
	VAO.LinkVertexBuffferObject(positions, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0, 0);					//coord
	VAO.LinkVertexBuffferObject(positions, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)), 0);    //colour
	VAO.LinkVertexBuffferObject(offsets, 2, 2, GL_FLOAT, 2 * sizeof(float), (void*)(0 * sizeof(float)), 2);	//offsets
	//Unbinds all Objects so that they aren't accidently modified
	VAO.Unbind();
	positions.Unbind();
	offsets.Unbind();
	EBO.Unbind();



	//set uniform values

	//loop

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //an RGB argument
		glClear(GL_COLOR_BUFFER_BIT);

	
		ShaderProgram.Activate();

		getInput(window, offset);

		offsets.UpdateData(sizeof(offset) / sizeof(float), GL_FLOAT, 0, offset);

		VAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(renderOrder) / (sizeof(GLuint)), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO.Delete();
	positions.Delete();
	offsets.Delete();
	ShaderProgram.Delete();
	EBO.Delete();

	clearProgram(window);

	return 0;
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
	glViewport(0, 0, windowPixelHeight, windowPixelHeight);
	return window;
}

void clearProgram(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "OpenGL program closed." << std::endl;
}

bool checkCollide(GameObject& r1,GameObject& r2)
{
	if ((r1.x + r1.width / 2 ) < (r2.x - r2.width / 2) ) return 0;
	if ((r1.x - r1.width / 2 ) > (r2.x + r2.width / 2) ) return 0;
	if ((r1.y + r1.height / 2) < (r2.y - r2.height / 2)) return 0;
	if ((r1.y - r1.height / 2) > (r2.y + r2.height / 2)) return 0;
	return 1;
}

void getInput(GLFWwindow* window, GLfloat* offset)
{
	std::cout << offset[0] << std::endl;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		offset[0] -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		offset[1] += 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		offset[0] += 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		offset[1] -= 0.001f;
	}

}