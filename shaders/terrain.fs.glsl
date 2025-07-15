#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D terrainTex;

uniform vec3 u_fogColor = vec3(0.949, 0.835, 0.659); 
uniform float u_fogDensity = 0.01;
uniform float u_fogStart = 100.0;
uniform float u_fogEnd = 250.0; 

float near = 0.1;
float far = 300.0;

float linearizeDepth(float depth) 
{
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth)
{
    float steepness = 0.015;
    float offset = 5.0;
    float zVal = linearizeDepth(depth);
    return (1 / (1 + exp(-steepness * (zVal - offset))));
}

vec3 applyDepthFog(vec3 color, float depth) {
    float linearDepth = linearizeDepth(depth);
    float fogFactor = clamp((u_fogEnd - linearDepth) / (u_fogEnd - u_fogStart), 0.0, 1.0);
    return mix(u_fogColor, color, fogFactor);
}

void main()
{
    vec4 texColor = texture(terrainTex, TexCoord);
    
    // float depth = logisticDepth(gl_FragCoord.z);
    // vec3 result = mix(texColor.rgb, u_fogColor, depth);
    
    vec3 result = applyDepthFog(texColor.rgb, gl_FragCoord.z);
    
    FragColor = vec4(result, texColor.a);
}
