#version 410
in data {
vec4 color ;
} f ;
out vec4 out_color;
void main ( void )
{
out_color = f.color;
}