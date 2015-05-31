#version 430

uniform mat4 MVP;

uniform sampler2D TextureSampler; 

in VertexData 
{
	vec3 Position;
	vec2 TextureCoord;
	vec3 Normal;
} Input;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture2D(TextureSampler, Input.TextureCoord);
}