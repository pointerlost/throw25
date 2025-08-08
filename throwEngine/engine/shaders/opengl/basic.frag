#version 440 core

out vec4 FragColor;

// take inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoords;

uniform uint activeLightCount;

// Scene Lighting stuff
struct Light {
    vec3 position;
    vec3 direction;

    vec3 diffuse;
    vec3 specular;

    // int type;
};
uniform Light lights[64];

uniform vec3 globalAmbient;
uniform bool isItLightVisualObject;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};
uniform Material material;

vec3 calcLightProperties(uint index, Material material, vec3 normal) {
    // ambient
    vec3 ambient = globalAmbient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lights[index].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lights[index].diffuse * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[index].specular * spec * material.specular;

    return ambient + diffuse + specular;
}

void main() {

    vec3 result = vec3(0.0);
    for (uint i = 0; i < activeLightCount; i++) {
        result += calcLightProperties(i, material, Normal);
    }
    FragColor = vec4(result, 1.0f);
}
