#version 400 core

uniform sampler2D diffuse;

uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
        float ambientStrength = 0.35f;
        float specularStrength = 0.1f;
        vec3 lightColor = vec3(1.0f);
        vec3 lightDir = normalize(FragPos - lightPos);
        vec3 cameraDir = normalize(FragPos - cameraPos);
        vec3 objectColor = texture(diffuse, TexCoords).rgb;

        vec3 ambientLight = lightColor * ambientStrength;
        vec3 diffuseLight = lightColor * clamp(abs(dot(lightDir, Normal)), 0.4f, 0.7f);
        vec3 specularLight = lightColor * specularStrength *
        pow(abs(dot(cameraDir, reflect(lightDir, Normal))), 64);

        FragColor = vec4((ambientLight + diffuseLight + specularLight) * objectColor, 1.0f);
}