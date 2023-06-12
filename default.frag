#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 colour;


void main()
{
	FragColor = vec4(colour, 1.0f);
}