#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;

// Por enquanto a gente pode texturas OU cores nesse fragmento
uniform bool useDiffuseMap;
uniform bool useSpecularMap;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

uniform vec3 u_fogColor = vec3(0.949, 0.835, 0.659);
uniform float u_fogDensity = 0.01;
uniform float u_fogStart = 50.0;
uniform float u_fogEnd = 150.0;

vec3 applyFog(vec3 color, vec3 worldPos, vec3 cameraPos) {
  float distance = length(worldPos - cameraPos);
  float fogFactor = clamp((u_fogEnd - distance) / (u_fogEnd - u_fogStart), 0.0, 1.0);
  return mix(u_fogColor, color, fogFactor);
}

void main() 
{
  vec4 albedo;
  if (useDiffuseMap) {
    albedo = texture(diffuse0, TexCoords);
  } else {
    albedo = vec4(material.diffuse, 1.0);
  }
  
  // alpha, tava dando problema em alguns modelos
  if (albedo.a <= 0.1)
    discard;

  vec3 specularColor;
  if (useSpecularMap) {
    specularColor = texture(specular0, TexCoords).rgb;
  } else {
    specularColor = material.specular;
  }

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - crntPos);
  vec3 viewDir = normalize(camPos - crntPos);

  // Ambiente
  vec3 ambient = min(material.ambient, 0.4) * lightColor.rgb;

  // Difusa
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor.rgb;
  
  // Especular
  vec3 specular = vec3(0.0);
  if (diff > 0.0) { 
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), max(material.shininess, 1.0));
    specular = spec * specularColor * lightColor.rgb;

    specular = min(specular, vec3(1.0));
  }

  vec3 result = (ambient + diffuse + specular) * albedo.rgb;
  vec3 finalColor = applyFog(result, crntPos, camPos);

  FragColor = vec4(finalColor, albedo.a);
}
