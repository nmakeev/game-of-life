#version 330 core
out vec4 color;

uniform vec4 quad_color;

void main()
{
    color = quad_color;
}