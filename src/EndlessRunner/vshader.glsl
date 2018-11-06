#version 410

layout (location = 0) in vec4 position;

uniform float scaling;
uniform vec4 translation;

void main()
{
    gl_Position = position * vec4(scaling, scaling, scaling, 1.0) + translation;
}
