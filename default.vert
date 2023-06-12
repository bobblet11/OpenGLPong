#version 330 core

// Positions/Coordinates
layout (location = 0) in vec2 positionData;
// Colors
layout (location = 1) in vec3 colourData;
// Delta X and Y
layout (location = 2) in vec2 offsets;

// Outputs the color for the Fragment Shader
out vec3 colour;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(positionData + offsets, 0.0, 1.0);

	// Assigns the colors from the Vertex Data to "color"
	colour = colourData;

}