#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 screenTransform;

void main()
{
    gl_Position = screenTransform * vec4(position.xy, 0, 1f);
}