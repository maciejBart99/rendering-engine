#version 330 core

struct Fog {
    vec3 color;
    float density;
};

out vec4 FragColor;

in vec3 TexCoords;
in vec4 Pos;

uniform samplerCube skybox;
uniform float lightness;
uniform Fog fog;


void main()
{
    FragColor = mix(texture(skybox, TexCoords) * clamp(vec4(lightness * 0.8, lightness * 0.8, lightness * 1.4, 1), 0, 1), vec4(fog.color, 1.0), clamp(fog.density * 20, 0, 1));
}