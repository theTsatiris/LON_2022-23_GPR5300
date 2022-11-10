#version 330 core

out vec4 FragColor;
in vec2 textureCoord;

uniform sampler2D textureObject1;
uniform sampler2D textureObject2;

void main()
{
	vec4 textureSample1 = texture(textureObject1, textureCoord);
	vec4 textureSample2 = texture(textureObject2, textureCoord);

	FragColor = mix(textureSample1, textureSample2, 0.7);
}