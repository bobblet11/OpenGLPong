#include "ShaderClass.h"

//reads the files and extracts its contents
std::string GetFileContents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//extract file contents in char array form of shaders
	std::string vertexCode = GetFileContents(vertexFile);
	std::string fragmentCode = GetFileContents(fragmentFile);
	const char* vertexSouce = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//create a vertex shader and compile it into machine code for GPU to understand, in the form of an integer ID reference
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexSouce, NULL);
	glCompileShader(vertexShaderID);

	//create a fragment shader and compile it into machine code for GPU to understand, in the form of a integer ID reference
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderID);

	//creates an empty shader program, attaches the compiled vertex shader and fragment shader, links program to the shader ID
	ID = glCreateProgram();
	glAttachShader(ID, vertexShaderID);
	glAttachShader(ID, fragmentShaderID);
	glLinkProgram(ID);

	//delete the individual shaders from memory since the program is already in memory
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::Activate()
{
	//next rendering uses this shader program
	glUseProgram(ID);
}

void Shader::Delete()
{
	//deletes the shader program from memory
	glDeleteProgram(ID);
}