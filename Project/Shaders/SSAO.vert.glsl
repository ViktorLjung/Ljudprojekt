#version 430

layout (location = 0) in vec3 Position;

out VertexData
{
	vec2 TextureCoord;
} Output;

void main()
{
	gl_Position = vec4(Position, 1.0);
	
	// Texture coordinate for screen aligned (in correct range: (0-1)
	Output.TextureCoord = (vec2(Position) + 1) / 2;
}
