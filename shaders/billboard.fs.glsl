#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
  vec4 texColor = texture(image, texCoords);
  if (texColor.a < 0.1)
    discard;

  color = vec4(spriteColor, 1.0) * texColor;
}
