#version 400

layout (location = 0) in vec4 position;

uniform float scaling;
uniform vec2 translation;

void main()
{
    gl_Position = position * vec4(scaling, scaling, scaling, 1.0) + translation;
}
