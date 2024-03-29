#include "Light.h"

Light::Light(float Raidus, float SpecularExponent, glm::vec3 Position, glm::vec3 DiffuseColor, glm::vec3 SpecularColor)
{
	m_Position = Position;
	m_Raidus = Raidus;
	m_SpecularExponent = SpecularExponent;
	m_DiffuseColor = DiffuseColor;
	m_SpecularColor = SpecularColor;
}

Light::Light()
{
	m_Position = glm::vec3(0.f, 20.0f, -70.f);
	m_Raidus = 300.f;
	m_SpecularExponent = 50.f;
	m_DiffuseColor = glm::vec3(0.5f, 0.5f, 1.f);
	
	//HACK: for some reason this fixes it, investigate in shaders sometime plz
	m_SpecularColor = glm::vec3(0.f); 
	
}
