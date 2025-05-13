#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Height;

uniform sampler2D terrainTex;

void main()
{
  float h = (Height + 16)/32.0f;

  float alpha = (h <= 0.1f) ? 0.0 : 1.0;

  FragColor = texture(terrainTex, TexCoord);
}
