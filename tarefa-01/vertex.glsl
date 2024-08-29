#version 410

layout (location=0) in vec4 coord;
layout (location=1) in vec4 color;
out data {
vec4 color;
} v;

void main ( void )
{
v.color = color;
gl_Position = coord ;
}