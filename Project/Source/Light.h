#include "PrecompiledHeader.h"

class Light
{
public:
	Light(float Raidus, float SpecularExponent, glm::vec3 Position, glm::vec3 DiffuseColor, glm::vec3 SpecularColor);
	Light();
	glm::vec3 Position() const { return m_Position; }
	void  Position(glm::vec3 val) { m_Position = val; }
	glm::vec3 DiffuseColor() const { return m_DiffuseColor; }
	void  DiffuseColor(glm::vec3 val) { m_DiffuseColor = val; }
	glm::vec3 SpecularColor() const { return m_SpecularColor; }
	void  SpecularColor(glm::vec3 val) { m_SpecularColor = val; }

	float  Radius() const { return m_Raidus; }
	void  Radius(float val) { m_Raidus = val; }
	float  SpecularExponent() const { return m_SpecularExponent; }
	void  SpecularExponent(float val) { m_SpecularExponent = val; }

	glm::mat4 ModelMatrix() 
	{ 
		glm::mat4 modelMatrix;
		modelMatrix *= glm::translate(m_Position);
		modelMatrix *= glm::scale(glm::vec3(m_Raidus * 2));
		return modelMatrix;
	}


private:
	float  m_Raidus, m_SpecularExponent;
	glm::vec3 m_Position, m_DiffuseColor, m_SpecularColor;

};
