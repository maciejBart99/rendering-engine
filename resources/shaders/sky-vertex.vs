#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out vec4 Pos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    Pos = projection * view * vec4(aPos, 1.0);
    gl_Position = Pos;
}