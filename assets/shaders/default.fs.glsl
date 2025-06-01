#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() 
{
    float ambientStrength = 0.20f;
    vec4 ambient = ambientStrength * lightColor;  

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - crntPos);

    float diffuse = max(dot(normal, lightDir), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 8);
    float specular = specAmount * specularLight;

    FragColor = texture(diffuse0, TexCoords) * (diffuse + ambient + specular); 
}
