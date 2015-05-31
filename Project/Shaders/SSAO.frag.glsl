#version 430

layout (binding=0) uniform sampler2D NormalTexture;
layout (binding=1) uniform sampler2D DepthTexture;
layout (binding=2) uniform sampler2D NoiseTexture;

uniform float TotalStrength;
uniform float Strength;
uniform float Offset;
uniform float Falloff;
uniform float Radius;

const int NUMBER_OF_SAMPLES = 16;
uniform vec3 Sample[NUMBER_OF_SAMPLES];

in VertexData
{
	vec2 TextureCoord;
} Input;

out vec4 frag_SSAO;



void main() 
{	
	// get the normal of current fragment
	vec3 normal =  texture2D(NormalTexture,  Input.TextureCoord).xyz;
	// get the depth of current fragment
	float depth = texture2D(DepthTexture,  Input.TextureCoord).x;
	// current fragment coords in screen space
	vec3 currentPos = vec3(Input.TextureCoord.x, Input.TextureCoord.y, depth);
	
	vec3 ray, se, occluderNormal;
	float occluderDepth, depthDifference, normalDifference;
	float occlusion = 0.0;
	
	vec2 noiseTexCoords = vec2(textureSize(DepthTexture, 0)) / vec2(textureSize(NoiseTexture, 0));
	noiseTexCoords *=  Input.TextureCoord;
	vec3 randomNormal = texture2D(NoiseTexture,  noiseTexCoords*Offset).xyz * 2.0 - 1.0;
	
	float radD = Radius * depth;
	
	for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
	{
		// Apply random rotation on the sample and multiply it with the radius
		  ray = radD * reflect(Sample[i], randomNormal);
	   
		// if the ray is outside the hemisphere, change direction
		se = currentPos + sign(dot(ray,normal) )*ray;
	  
		// get the normal of the occluder fragment
		occluderNormal = texture2D(NormalTexture,  se.xy).xyz;
		occluderDepth = texture2D(DepthTexture, se.xy).x;
		
		// if depthDifference is negative, occluder is behind current fragment
		depthDifference = depth - occluderDepth;
		normalDifference = (1.0 - dot(occluderNormal, normal));

		occlusion += step(Falloff, depthDifference) * normalDifference * (1.0 - smoothstep( Falloff, Strength, depthDifference));
      
	}
	
	float finalAO = 1.0 - (occlusion * TotalStrength / NUMBER_OF_SAMPLES);
	
	frag_SSAO = vec4(finalAO, 0, 0, 0);
}



//https://www.opengl.org/sdk/docs/man/html/sign.xhtml
//https://www.opengl.org/sdk/docs/man/html/step.xhtml
//https://www.opengl.org/sdk/docs/man/html/smoothstep.xhtml