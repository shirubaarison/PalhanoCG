#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vertexColor = vec4(aColor, 1.0);
  gl_Position = projection * view * vec4(FragPos, 1.0);
}
