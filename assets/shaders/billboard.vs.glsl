#version 330 core
layout (location = 0) in vec4 vertex; // pos e texutras

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float billboardScale;

void main()
{
  texCoords = vertex.zw;

  vec3 billboardCenterWorld = model[3].xyz;

  vec4 billboardCenterView = view * vec4(billboardCenterWorld, 1.0);

  vec4 finalPosView = billboardCenterView;
  finalPosView.x += vertex.x * billboardScale;
  finalPosView.y += vertex.y * billboardScale;

  gl_Position = projection * finalPosView;
}
