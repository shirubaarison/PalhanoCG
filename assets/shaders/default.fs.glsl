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

void main() {
  vec4 albedo;
  if (useDiffuseMap) {
    albedo = texture(diffuse0, TexCoords);
  } else {
    albedo = vec4(material.diffuse, 1.0);
  }

  vec3 specularColor;
  if (useSpecularMap) {
    specularColor = texture(specular0, TexCoords).rgb;
  } else {
    specularColor = material.specular;
  }

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - crntPos);
  vec3 viewDir = normalize(camPos - crntPos);

  // Iluminação ambiente
  vec3 ambient = max(material.ambient, vec3(0.1)) * lightColor.rgb;

  // Iluminação difusa
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor.rgb;

  // Iluminação especular
  vec3 specular = vec3(0.0);
  
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec_factor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  specular = spec_factor * specularColor * lightColor.rgb;

  vec3 result = (ambient + diffuse + specular) * albedo.rgb;

  FragColor = vec4(result, albedo.a);
}
