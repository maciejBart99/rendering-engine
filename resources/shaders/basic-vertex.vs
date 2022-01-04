#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec4 ViewSpacePos;
out vec4 FragPos;
out vec3 Normal;
out vec3 Tangent;
out mat3 TBN;
out vec2 TexCoords;
out vec3 ShColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

void main()
{
    FragPos = model * vec4(aPos, 1.0);
    Normal = normalize(vec3(model * vec4(aNormal, 0.0)));
    Tangent = normalize(vec3(model * vec4(aTangent, 0.0)));

    TBN = mat3(Tangent, normalize(cross(Normal, Tangent)), Normal);
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ViewSpacePos = view * model * vec4(aPos, 1.0);
    ShColor = vec3(0, 0, 0);
}