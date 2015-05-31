#include "Rectangle.h"


Rectangle::Rectangle()
{
	m_Position = glm::vec3(0);
	m_Orientation = glm::quat();

		
	m_Vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	m_Vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	m_Vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	m_Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	m_TextureCoords.push_back(glm::vec2(0.0f, 0.0f));
	m_TextureCoords.push_back(glm::vec2(1.0f, 0.0f));
	m_TextureCoords.push_back(glm::vec2(0.0f, 1.0f));
	m_TextureCoords.push_back(glm::vec2(1.0f, 1.0f));

	m_Normals.push_back(glm::vec3(0.f, 0.f, 1.0f));
	m_Normals.push_back(glm::vec3(0.f, 0.f, 1.0f));
	m_Normals.push_back(glm::vec3(0.f, 0.f, 1.0f));
	m_Normals.push_back(glm::vec3(0.f, 0.f, 1.0f));
}

glm::mat4 Rectangle::ModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.f);
	return modelMatrix;
}

void Rectangle::CreateBuffers()
{
	if (m_Vertices.size() > 0)
	{
		LOG_INFO("Generating m_VertexBuffer");
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), &m_Vertices[0], GL_STATIC_DRAW);
		GLERROR("m_VertexBuffer FAIL");
	}
	else
	{
		LOG_WARNING("m_VertexBuffer is empty");
	}

	if (m_TextureCoords.size() > 0)
	{
		LOG_INFO("Generating m_TextureCoordBuffer");
		glGenBuffers(1, &m_TextureCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_TextureCoords.size() * sizeof(glm::vec2), &m_TextureCoords[0], GL_STATIC_DRAW);
		GLERROR("m_TextureCoordBuffer FAIL");
	}
	else
	{
		LOG_WARNING("m_TextureCoordBuffer is empty");
	}

	if (m_Normals.size() > 0)
	{
		LOG_INFO("Generating m_NormalBuffer");
		glGenBuffers(1, &m_NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Normals.size() * sizeof(glm::vec3), &m_Normals[0], GL_STATIC_DRAW);
		GLERROR("m_NormalBuffer FAIL");
	}
	else
	{
		LOG_WARNING("m_NormalBuffer is empty");
	}

	m_Texture = SOIL_load_OGL_texture("Assets/Textures/Cool.png", 0, 0, SOIL_FLAG_INVERT_Y);

	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Rectangle::Bind()
{
	glBindVertexArray(m_VertexArrayObject);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}


