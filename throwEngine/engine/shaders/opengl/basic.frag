#version 440 core

in vec3 normal;
in vec3 color;
in vec2 texCoords;
in vec3 FragPos;

out vec4 fragColor;

// Light Types
const int LIGHT_POINT = 0;
const int LIGHT_DIRECTIONAL = 1;
const int LIGHT_SPOT = 2;

// Scene Lighting
struct Light {
    vec3 position;
    vec3 direction;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    int type;
};

uniform Light lights[256];
uniform uint activeLightCount;
uniform vec3 globalAmbient;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;

uniform sampler2D material_diffuseTex;
uniform sampler2D material_specularTex;

uniform bool hasDiffuseTex;
uniform bool hasSpecularTex;

vec3 CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir;
    float attenuation = 1.0;
    float intensity = 1.0;

    if (light.type == LIGHT_DIRECTIONAL) {
        lightDir = normalize(-light.direction);
    } else {
        lightDir = normalize(light.position - fragPos);

        // attenuation (point and spot lights)
        float distance = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // spotlight intensity
        if (light.type == LIGHT_SPOT) {
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.cutOff - light.outerCutOff;
            intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        }
    }

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // Apply texture colors if present
    vec3 diffuseColor = material.diffuse;
    if (hasDiffuseTex)
        diffuseColor *= texture(material_diffuseTex, texCoords).rgb;

    vec3 specularColor = material.specular;
    if (hasSpecularTex)
        specularColor *= texture(material_specularTex, texCoords).rgb;

    vec3 ambient = globalAmbient * material.ambient;
    vec3 diffuse = light.diffuse * diffuseColor * diff;
    vec3 specular = light.specular * specularColor * spec;

    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    for (uint i = 0; i < activeLightCount; ++i) {
        result += CalcLight(lights[i], norm, FragPos, viewDir);
    }

    fragColor = vec4(result * color, 1.0);
}
