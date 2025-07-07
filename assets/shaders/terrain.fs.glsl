// #version 330 core
// out vec4 FragColor;

// in vec2 TexCoord;

// uniform sampler2D terrainTex;

// float near = 0.1f;
// float far = 300.0f;

// float linearizeDepth(float depth) 
// {
//     return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
// }

// float logisticDepth(float depth)
// {
//   float steepness = 0.02f;
//   float offset = 3.0f;
//   float zVal = linearizeDepth(depth);
//   return (1 / (1 + exp(-steepness * (zVal - offset))));
// }

// void main()
// {
//   float depth = logisticDepth(gl_FragCoord.z);
//   FragColor = texture(terrainTex, TexCoord) * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
// }

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D terrainTex;

void main()
{
  FragColor = texture(terrainTex, TexCoord);
}
