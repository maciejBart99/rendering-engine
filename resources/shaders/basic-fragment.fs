#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D kdMap;
    sampler2D ksMap;
    sampler2D nMap;
    int hasNMap;
    int hasKdMap;
    int hasKsMap;
    float opacity;
    vec3 Ka;
    vec3 Ks;
    vec3 Kd;
    int shiness;
};

struct Fog {
    vec3 color;
    float start;
    float end;
    float density;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
};

#define MAX_LIGHTS 16

in vec4 ViewSpacePos;
in vec4 FragPos;
in vec3 Normal;
in vec3 Tangent;
in mat3 TBN;
in vec2 TexCoords;
in vec3 ShColor;

uniform Fog fog;
uniform int useGouraud;
uniform int useBlinn;
uniform int useLights;
uniform vec3 cameraPosition;
uniform int pointLightsCount;
uniform int dirLightsCount;
uniform int spotLightsCount;
uniform Light pointLights[MAX_LIGHTS];
uniform Light dirLights[MAX_LIGHTS];
uniform Light spotLights[MAX_LIGHTS];
uniform Material material;

float CalcFogFactor(Fog fog, vec4 viewSpacePos)
{
    float fogLength = fog.end - fog.start;
    return 1.0 - clamp(exp(-pow(fog.density * viewSpacePos.z / viewSpacePos.w, 2)), 0.0, 1.0);
}

mat3 CalcLightMatrix(Light light, vec3 L, vec3 normal, vec3 fragPos, vec3 camera, Material mat, int blinn)
{
    vec3 N;
    if (mat.hasNMap == 1) {
        vec3 NC = texture(mat.nMap, TexCoords).rgb;

        N = vec3(NC.x * 2 - 1, NC.y * 2 - 1, NC.z);
    } else {
        N = vec3(0, 0, 1);
    }

    N = normalize(TBN * N);

    vec3 LT = L;
    vec3 VT = normalize(camera - fragPos);

    vec3 ambient = mat.Ka * light.ambient;
    vec3 diffuse;
    vec3 specular;

    if (mat.hasKdMap == 1) {
        diffuse = vec3(texture(material.kdMap, TexCoords));
    } else {
        diffuse = mat.Kd;
    }

    ambient = ambient * diffuse;

    if (mat.hasKsMap == 1) {
        specular = vec3(texture(material.ksMap, TexCoords));
    } else {
        specular = mat.Ks;
    }

    diffuse = diffuse * max(dot(N, LT), 0) * light.diffuse;

    if (blinn == 0) {
        vec3 R = reflect(-LT, N);
        specular = specular * light.diffuse * pow(max(dot(R, VT), 0), mat.shiness * 8);
    } else {
        vec3 H = normalize(LT + VT);
        specular = specular * light.diffuse * pow(max(dot(H, N), 0), mat.shiness * 2);
    }

    return mat3(ambient, diffuse, specular);
}

mat3 CalcDirLight(Light light, vec3 normal, vec3 fragPos, vec3 camera, Material mat, int blinn)
{
    return CalcLight(light, normalize(-light.direction), normal, fragPos, camera, mat, blinn);
}

mat3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 camera, Material mat, int blinn)
{
    mat3 color = CalcLightMatrix(light, normalize(light.position - fragPos), normal, fragPos, camera, mat, blinn);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return color * attenuation;
}

mat3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 camera, Material mat, int blinn)
{
    vec3 lightDir = normalize(light.position - fragPos);
    mat3 color = CalcLightMatrix(light, normalize(light.position - fragPos), normal, fragPos, camera, mat, blinn);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return color * attenuation * intensity;
}

void main()
{
    if (useGouraud == 0) {
        mat3 result = mat3(0);

        if (useLights == 1) {
            vec3 norm = normalize(Normal);

            for (int i = 0; i < dirLightsCount; i++)
                result += CalcDirLight(dirLights[i], norm, vec3(FragPos), cameraPosition, material, useBlinn);

            for (int i = 0; i < pointLightsCount; i++)
                result += CalcPointLight(pointLights[i], norm, vec3(FragPos), cameraPosition, material, useBlinn);

            for (int i = 0; i < spotLightsCount; i++)
                result += CalcSpotLight(spotLights[i], norm, vec3(FragPos), cameraPosition, material, useBlinn);
        } else {
            if (material.hasKdMap == 1) {
                vec3 diffuse = vec3(texture(material.kdMap, TexCoords));
                result = mat3(diffuse, diffuse, diffuse);
            } else {
                result = mat3(material.Kd, material.Kd, material.Kd);
            }
        }

        float fg = CalcFogFactor(fog, ViewSpacePos);
        vec4 o = vec4(result, 1);
        vec4 fc = vec4(fog.color, 1.0);

        FragColor = mix(o, fc, fg);
    } else {
        FragColor = vec4(ShColor, 1);
    }
}