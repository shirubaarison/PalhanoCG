#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;
uniform vec3 u_fogColor = vec3(0.949, 0.835, 0.659);
uniform float u_fogStart = 100.0;
uniform float u_fogEnd = 150.0;

vec3 applySkyboxFog(vec3 color, vec3 direction) {
  float horizonDistance = (1.0 - abs(direction.y)) * 100.0 + 50.0;

  float fogFactor = clamp((u_fogEnd - horizonDistance) / (u_fogEnd - u_fogStart), 0.0, 1.0);
  return mix(u_fogColor, color, fogFactor);
}

void main()
{
  vec4 skyboxColor = texture(skybox, TexCoords);

  vec3 result = applySkyboxFog(skyboxColor.rgb, normalize(TexCoords));

  FragColor = vec4(result, skyboxColor.a);
}
