#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 crntPos;
out vec4 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
  // Posição em world space
  crntPos = vec3(model * vec4(aPos, 1.0));

  // Calcular normal em world space
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  Normal = normalMatrix * aNormal;

  // Normalizar aqui para garantir
  Normal = normalize(Normal);

  TexCoords = aTexCoords;
  VertexColor = aColor;

  gl_Position = projection * view * vec4(crntPos, 1.0);
}
