#version 400 core

layout (location = 0) in vec3 position;

uniform float uAspect;
out vec3 pos;

void main()
{
    pos = position;
    //pos.x /= uAspect;
    gl_Position = vec4(pos, 1.0f);
}
