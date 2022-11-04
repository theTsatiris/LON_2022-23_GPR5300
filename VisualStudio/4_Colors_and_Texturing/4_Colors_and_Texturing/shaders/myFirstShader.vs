#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 texCoord;

out vec4 vertexColor;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoord = texCoord;
	gl_Position = projection * view * model * vec4(position, 1.0);
	vertexColor = vec4(colour, 1.0);
}