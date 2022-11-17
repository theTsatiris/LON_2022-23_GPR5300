#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPosition = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;
		
	textureCoord = texCoord;
	gl_Position = projection * view * vec4(FragPosition, 1.0);
}