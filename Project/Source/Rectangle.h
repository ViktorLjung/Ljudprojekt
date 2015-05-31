#include "PrecompiledHeader.h"
#include "SOIL.h"
#include <vector>

class Rectangle
{
public:
	Rectangle();

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::quat m_Orientation;
	

	GLuint m_VertexArrayObject;
	
	void CreateBuffers();
	void Draw(double dt);
	void Bind();

	glm::mat4 ModelMatrix();

	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec2> m_TextureCoords;
	std::vector<glm::vec3> m_Normals;
private:
	GLuint m_VertexBuffer;
	GLuint m_TextureCoordBuffer;
	GLuint m_NormalBuffer;
	GLuint m_Texture;
	
};