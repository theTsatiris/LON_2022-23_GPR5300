#version 330 core

struct Material
{
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 FragColor;

in vec2 textureCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D textureObject1;
uniform sampler2D textureObject2;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform Material surfaceMaterial;

uniform bool useBlinnPhong;
uniform bool useMaterial;

void main()
{
	vec3 surfaceColor; 
	if(useMaterial)
	{
		surfaceColor = surfaceMaterial.color;
	}
	else
	{
		vec4 textureSample1 = texture(textureObject1, textureCoord);
		vec4 textureSample2 = texture(textureObject2, textureCoord);
		surfaceColor = vec3(mix(textureSample1, textureSample2, 0.7));
	}

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	//ambient component
	ambient = surfaceColor * surfaceMaterial.ambient * lightColor;

	//diffuse component
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	diffuse = surfaceColor * surfaceMaterial.diffuse * lightColor * max(dot(normal, lightDirection), 0.0);

	//specular component
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	if(useBlinnPhong)
	{
		vec3 halfwayVector = normalize(lightDirection + viewDirection);
		specular = surfaceColor * surfaceMaterial.specular * lightColor * pow(dot(halfwayVector, normal), surfaceMaterial.shininess);
	}
	else
	{
		vec3 reflectionDir = reflect(-lightDirection, normal);
		specular = surfaceColor * surfaceMaterial.specular * lightColor * pow(max(dot(viewDirection, reflectionDir), 0.0), surfaceMaterial.shininess);
	}
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}