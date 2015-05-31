#version 430

uniform sampler2D TextureInput;

const float blurSize = 1.0/1280.0;
in vec2 TexCoord;

out vec4 fResult;

void main()
{
   vec4 sum = vec4(0.0);

   sum += texture2D(TextureInput, vec2(TexCoord.x - 4.0*blurSize, TexCoord.y)) * 0.05;
   sum += texture2D(TextureInput, vec2(TexCoord.x - 3.0*blurSize, TexCoord.y)) * 0.09;
   sum += texture2D(TextureInput, vec2(TexCoord.x - 2.0*blurSize, TexCoord.y)) * 0.12;
   sum += texture2D(TextureInput, vec2(TexCoord.x - blurSize, TexCoord.y)) * 0.15;
   sum += texture2D(TextureInput, vec2(TexCoord.x, TexCoord.y)) * 0.16;
   sum += texture2D(TextureInput, vec2(TexCoord.x + blurSize, TexCoord.y)) * 0.15;
   sum += texture2D(TextureInput, vec2(TexCoord.x + 2.0*blurSize, TexCoord.y)) * 0.12;
   sum += texture2D(TextureInput, vec2(TexCoord.x + 3.0*blurSize, TexCoord.y)) * 0.09;
   sum += texture2D(TextureInput, vec2(TexCoord.x + 4.0*blurSize, TexCoord.y)) * 0.05;
 
   fResult = sum;
}

