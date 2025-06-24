
#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;
in vec4 VertexColor;

out vec4 FragColor;

uniform bool useVertexColors;
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
  vec3 normal = normalize(Normal);
  vec3 lightVec = lightPos - crntPos;
  vec3 lightDir = normalize(lightVec);
  vec3 viewDir = normalize(camPos - crntPos);
  
  float distance = length(lightVec);
  float attenuation = 1.0 / (0.05 * distance * distance + 0.1 * distance + 1.0);
  
  vec3 baseColor = useVertexColors ? VertexColor.rgb : texture(diffuse0, TexCoords).rgb;
  vec3 specMap = max(texture(specular0, TexCoords).rgb, vec3(0.05));

  float diff = max(dot(normal, lightDir), 0.0);
  
  vec3 ambient = material.ambient * lightColor.rgb * baseColor;
  vec3 diffuse = lightColor.rgb * baseColor * diff * attenuation;
  
  vec3 specular = vec3(0.0);
  if (diff > 0.0) {
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = material.specular * specMap * lightColor.rgb * spec * attenuation;
  }

  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
