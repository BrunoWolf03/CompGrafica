#version 410 core

layout(location = 0) in vec2 aPos;

uniform mat4 M; // Transformation matrix

void main(void)
{
    gl_Position = M * vec4(aPos, 0.0, 1.0);
}