#version 430

layout (location = 0) in vec3 Position;

out	vec2 TexCoord;

void main()
{
	gl_Position = vec4(Position, 1.0);
	TexCoord = (vec2(Position) + 1) / 2;
}
