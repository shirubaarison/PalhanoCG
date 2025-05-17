#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D terrainTex;

void main()
{
  FragColor = texture(terrainTex, TexCoord);
}
