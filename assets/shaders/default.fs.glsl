#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuse0;

void main() 
{
    FragColor = texture(diffuse0, TexCoords); 
}
