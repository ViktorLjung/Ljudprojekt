#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
uniform mat4 V;

in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
	vec3 Normal;
	vec3 SSAONormal;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;
out vec4 frag_SSAONormal;


void main()
{
	// Position
	frag_Position = vec4(Input.Position.xyz, 1.0);

	// Normal
	frag_Normal = vec4(Input.Normal, 1.0);

	// Diffuse Texture
	//frag_Diffuse = tex;
	frag_Diffuse = texture(DiffuseTexture, Input.TextureCoord);

	// SSAONormal
	frag_SSAONormal = vec4(Input.SSAONormal, 1.0);
}