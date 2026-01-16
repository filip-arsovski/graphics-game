#version 400 core

in vec3 pos; // fragment position
out vec4 fragColor;

uniform float playerHealth; // health from 0 to 100;
uniform float barWidth;
uniform float leftmostBarXpos;
uniform float uAspect;

void main() {
    if(pos.x <= leftmostBarXpos + playerHealth*0.01*barWidth)
        fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}