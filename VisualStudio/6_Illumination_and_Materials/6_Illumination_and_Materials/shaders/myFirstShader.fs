#version 330 core

out vec4 FragColor;

in vec2 textureCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D textureObject1;
uniform sampler2D textureObject2;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

//uniform bool useBlinn;

void main()
{
	vec4 textureSample1 = texture(textureObject1, textureCoord);
	vec4 textureSample2 = texture(textureObject2, textureCoord);
	vec3 surfaceColor = vec3(mix(textureSample1, textureSample2, 0.7));

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	//ambient component
	ambient = surfaceColor * 0.1 * lightColor;
	//0.1 = mock ambient coefficient

	//diffuse component
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	diffuse = surfaceColor * 0.6 * lightColor * max(dot(normal, lightDirection), 0.0);
	//0.6 = mock diffuse coefficient

	//specular component
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	specular = surfaceColor * 0.7 * lightColor * pow(max(dot(viewDirection, reflectionDir), 0.0), 32);
	//0.7, 32 = mock specular coefficient and shininess respectively
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}