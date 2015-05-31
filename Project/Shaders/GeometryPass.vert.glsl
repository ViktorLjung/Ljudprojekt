#version 430

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TextureCoord;
layout (location = 2) in vec3 Normal;


out VertexData
{
	vec3 Position;
	vec2 TextureCoord;
	vec3 Normal;
	vec3 SSAONormal;
} Output;

void main()
{
	gl_Position = P*V*M * vec4(Position, 1.0);
	Output.TextureCoord = TextureCoord;
	Output.Position = vec3(V * M * vec4(Position, 1.0));
	Output.SSAONormal = normalize(vec3(P * V * M * vec4(Normal, 0.0))); // ScreenSpace normals needed for SSAO
	Output.Normal = normalize(vec3(inverse(transpose(V * M)) * vec4(Normal, 0.0)));
}