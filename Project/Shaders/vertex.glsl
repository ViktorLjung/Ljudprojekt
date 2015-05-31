#version 430

uniform mat4 MVP;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;
layout(location = 2) in vec3 Normal;

out VertexData 
{
	vec3 Position;
	vec2 TextureCoord;
	vec3 Normal;
} Output;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);

	Output.Position = Position;
	Output.TextureCoord = TextureCoord;
	Output.Normal = Normal;
}