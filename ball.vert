#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 positionData;
// Colors
layout (location = 1) in vec3 colourData;
// Texture Coordinates
layout (location = 2) in vec2 textureData;


// Outputs the color for the Fragment Shader
out vec3 colour;

// Outputs the texture coordinates to the fragment shader
out vec2 textureCoordinate;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

	// Assigns the colors from the Vertex Data to "color"
	colour = colourData;

	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	textureCoordinate = textureData;
}