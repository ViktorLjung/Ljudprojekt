#version 430

uniform sampler2D DiffuseTexture; 

in VertexData 
{
	vec2 TextureCoord;
} Input;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture2D(DiffuseTexture, Input.TextureCoord);
}

