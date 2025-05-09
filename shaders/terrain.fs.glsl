#version 330 core
out vec4 FragColor;

in float Height;

void main()
{
  float h = (Height + 16)/32.0f;

  float alpha = (h <= 0.1f) ? 0.0 : 1.0;

  FragColor = vec4(h, h, h, alpha);
}
