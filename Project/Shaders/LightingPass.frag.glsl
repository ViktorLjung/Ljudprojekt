#version 430

layout (binding=0) uniform sampler2D PositionTexture;
layout (binding=1) uniform sampler2D NormalsTexture;

uniform vec2 ViewportSize;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 ls;
uniform vec3 ld;
uniform vec3 lp;
uniform float specularExponent;
uniform vec3 CameraPosition;
uniform float LightRadius;

const vec3 ks = vec3(1.0, 1.0, 1.0);
const vec3 kd = vec3(1.0, 1.0, 1.0);
const vec3 ka = vec3(1.0, 1.0, 1.0);
const float kshine = 1.0;
const vec3 La = vec3(0.3, 0.3, 0.3);

in VertexData
{
 vec3 Position;
 vec2 TextureCoord;
} Input;

out vec4 frag_Light;

vec4 phong(vec3 position, vec3 normal, vec3 specular)
{
 // Diffuse
 vec3 lightPos = vec3(V * vec4(lp, 1.0));
 vec3 distanceToLight = lightPos - position;
 vec3 directionToLight = normalize(distanceToLight);
 float dotProd = dot(directionToLight, normal);
 dotProd = max(dotProd, 0.0);
 vec3 Id = kd * ld * dotProd;

 // Specular
 vec3 surfaceToViewer = normalize(-position);
 vec3 halfWay = normalize(surfaceToViewer + directionToLight);
 float dotSpecular = max(dot(halfWay, normal), 0.0);
 float specularFactor = pow(dotSpecular, specularExponent);
 vec3 Is = specular.r * ls * specularFactor;

 //Attenuation
 float dist = distance(lightPos, position);

 float attenuation = pow(max(0.0f, 1.0 - (dist / LightRadius)), 2);

 return vec4((Id) * attenuation, Is.r * attenuation);
}


void main()
{
 vec2 TextureCoord = gl_FragCoord.xy / ViewportSize;
 vec4 PositionTexel = texture(PositionTexture, TextureCoord);
 vec4 NormalTexel = texture(NormalsTexture, TextureCoord);

 frag_Light = phong(vec3(PositionTexel), vec3(NormalTexel), vec3(1.0));
}