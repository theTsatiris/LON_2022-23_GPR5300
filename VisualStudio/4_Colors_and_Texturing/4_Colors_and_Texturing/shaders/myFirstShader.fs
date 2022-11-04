#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 textureCoord;

uniform sampler2D textureObject;

void main()
{
	FragColor = texture(textureObject, textureCoord);
	//FragColor = vertexColor;
}