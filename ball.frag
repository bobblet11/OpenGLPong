#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 colour;

// Inputs the texture coordinates from the Vertex Shader
in vec2 textureCoordinate;

// Gets the Texture Unit from the main function
uniform sampler2D textureSampler;


void main()
{
	FragColor = texture(textureSampler, textureCoordinate);
}