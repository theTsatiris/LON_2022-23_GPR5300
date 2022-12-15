#version 330 core

out vec4 FragColor;

//Material
uniform float ambientCoefficient;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 specularModifier;
uniform float shininess;
//--------

//Input from Vertex Shader
in vec2 textureCoord;
in vec3 Normal;
in vec3 FragPosition;
//------------------------

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform samplerCube skybox;

uniform bool useBlinnPhong;

vec3 reflection()
{
    vec3 viewDirection = normalize(FragPosition - viewPosition);
    vec3 normal = normalize(Normal);

    vec3 reflectionDir = reflect(viewDirection, normal);

    return texture(skybox, reflectionDir).rgb;
}

void main()
{
    vec3 diffuseValues = texture(texture_diffuse1, textureCoord).rgb;
    vec3 specularValues = texture(texture_specular1, textureCoord).rgb * specularModifier;

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    //ambient component
    ambient = diffuseValues * ambientCoefficient * lightColor;

    //diffuse component
    vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
    diffuse = diffuseValues * lightColor * max(dot(normal, lightDirection), 0.0);

    //specular component
    vec3 viewDirection = normalize(viewPosition - FragPosition); 
    if(useBlinnPhong)
    {
        vec3 halfwayVector = normalize(viewDirection + lightDirection);
        specular = specularValues * lightColor * pow(max(dot(halfwayVector, normal), 0.0), shininess);
    }
    else
    {
        vec3 reflectionDir = reflect(-lightDirection, normal);
        specular = specularValues * lightColor * pow(max(dot(viewDirection, reflectionDir), 0.0), shininess);
    }

    vec3 result = ambient + diffuse + (specular * reflection());
    FragColor = vec4(result, 1.0);
}