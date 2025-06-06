#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos; 
    float dist = length(lightVec);
    float a = 3.0;
    float b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
    
    float ambientStrength = 0.20f;
    vec4 ambient = ambientStrength * lightColor;  

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);

    float diffuse = max(dot(normal, lightDir), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 8);
    float specular = specAmount * specularLight;

    return texture(diffuse0, TexCoords) * (diffuse * inten + ambient) + texture(specular0, TexCoords).r * specular * inten; 
}

vec4 directLight()
{
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 0.05;
    float b = 0.1;
    float inten = 1.0 / (a * dist * dist + b * dist + 1.0);

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);

    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);

    vec4 texColor = texture(diffuse0, TexCoords);
    float specTex = texture(specular0, TexCoords).r;

    vec4 ambient = 0.03 + inten * lightColor * texColor;
    float specular = spec * specTex * 0.5 * inten;

    return texColor * diffuse * inten + ambient + vec4(lightColor.rgb * specular, texColor.a);
}

vec4 spotLight()
{
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    vec3 lightVec = lightPos - crntPos;
    
    float ambientStrength = 0.20f;
    vec4 ambient = ambientStrength * lightColor;  

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);

    float diffuse = max(dot(normal, lightDir), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 8);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return texture(diffuse0, TexCoords) * (diffuse * inten + ambient) + texture(specular0, TexCoords).r * specular * inten; 
}

void main() 
{   
    FragColor = directLight();
}
