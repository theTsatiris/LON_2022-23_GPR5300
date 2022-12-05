#version 330 core

struct Material
{
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};//material with rgb values

struct LMapMaterial
{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};//light map material

out vec4 FragColor;

in vec2 textureCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform Material surfaceMaterial;
uniform LMapMaterial mapMaterial;

uniform bool useBlinnPhong;
uniform bool useMaterial;

uniform samplerCube skybox;

vec3 reflection()
{
    vec3 viewDirection = normalize(FragPosition - viewPosition);
    vec3 normal = normalize(Normal);

    vec3 reflectionDir = reflect(viewDirection, normal);

    return texture(skybox, reflectionDir).rgb;
}

void main()
{
	vec3 surfaceColor;
	vec3 surfaceAmbient;
	vec3 surfaceDiffuse;
	vec3 surfaceSpecular;
	float surfaceShininess;

	if(useMaterial) // using "traditional" rgb material
	{
		surfaceColor = surfaceMaterial.color;
		surfaceAmbient = surfaceMaterial.ambient;
		surfaceDiffuse = surfaceMaterial.diffuse;
		surfaceSpecular = surfaceMaterial.specular;
		surfaceShininess = surfaceMaterial.shininess;
	}
	else // using light maps
	{
		//surfaceColor = vec3(1.0);
		surfaceAmbient = mapMaterial.ambient;
		surfaceDiffuse = texture(mapMaterial.diffuse, textureCoord).rgb;
		surfaceColor = surfaceDiffuse;
		surfaceSpecular = texture(mapMaterial.specular, textureCoord).rgb;
		surfaceShininess = mapMaterial.shininess;		
	}

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	//ambient component
	ambient = surfaceColor * surfaceAmbient * lightColor;

	//diffuse component
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	diffuse = surfaceDiffuse * lightColor * max(dot(normal, lightDirection), 0.0);

	//specular component
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	if(useBlinnPhong)
	{
		vec3 halfwayVector = normalize(lightDirection + viewDirection);
		specular = surfaceColor * surfaceSpecular * lightColor * pow(dot(halfwayVector, normal), surfaceShininess);
	}
	else
	{
		vec3 reflectionDir = reflect(-lightDirection, normal);
		specular = surfaceColor * surfaceSpecular * lightColor * pow(max(dot(viewDirection, reflectionDir), 0.0), surfaceShininess);
	}
	
	vec3 result = ambient + diffuse + (specular * reflection());
	FragColor = vec4(result, 1.0);
}