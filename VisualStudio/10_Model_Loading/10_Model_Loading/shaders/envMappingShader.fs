#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;

uniform vec3 viewPosition;

uniform samplerCube skybox;

uniform float extMediumIdx;
uniform float entMediumIdx;

vec3 reflection()
{
    vec3 viewDirection = normalize(FragPosition - viewPosition);
    vec3 normal = normalize(Normal);

    vec3 reflectionDir = reflect(viewDirection, normal);

    return texture(skybox, reflectionDir).rgb;
}

vec3 refraction()
{
    vec3 viewDirection = normalize(FragPosition - viewPosition);
    vec3 normal = normalize(Normal);

    float ratio = extMediumIdx / entMediumIdx; 
    vec3 refractionDir = refract(viewDirection, normal, ratio);

    return texture(skybox, refractionDir).rgb;
}

void main()
{
    vec3 color = refraction();
    FragColor = vec4(color, 1.0);
}