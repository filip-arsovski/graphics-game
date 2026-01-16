#version 400 core

uniform sampler2D diffuse;
uniform vec3 lightPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuse, TexCoords);
}