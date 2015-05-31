#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D LightingTexture;
layout (binding=2) uniform sampler2D SSAOTexture;


in VertexData
{
 vec2 TextureCoord;
} Input;

out vec4 final_color;

void main()
{
float SSAO = texture(SSAOTexture, Input.TextureCoord).r;

 vec4 diffuseTexel = texture(DiffuseTexture, Input.TextureCoord);
 vec4 lightingTexel = texture(LightingTexture, Input.TextureCoord);
 vec4 FinalColor = diffuseTexel * lightingTexel * SSAO;

 final_color = FinalColor;
}