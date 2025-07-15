#version 330 core
layout (location = 0) in vec4 vertex; // pos e texutras

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;
out vec4 diffuseColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float billboardScale;

// Lighting uniforms
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
  texCoords = vertex.zw;

  vec3 billboardCenterWorld = model[3].xyz;
  vec4 billboardCenterView = view * vec4(billboardCenterWorld, 1.0);
  vec4 finalPosView = billboardCenterView;
  finalPosView.x += vertex.x * billboardScale;
  finalPosView.y += vertex.y * billboardScale;

  vec4 worldPos = inverse(view) * finalPosView;
  fragPos = worldPos.xyz;

  normal = normalize(camPos - fragPos);

  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  diffuseColor = lightColor * diff;

  gl_Position = projection * finalPosView;
}
