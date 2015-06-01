#pragma once
#include "PrecompiledHeader.h"
#include <vector>
#include "SOIL.h"
#include <fstream>
#include <sstream>
#include <tuple>
#include <map>

class Model
{
public:
	Model(std::string path);
	void ModelMatrix(glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	void ModelMatrix(glm::mat4 trans);
	glm::mat4 ModelMatrix();
	
	void CreateBuffers();
	void Bind();
	
	int NumberOfVertices();
	GLuint m_VertexArrayObject;
	GLuint m_Texture;

private:
	bool LoadOBJ(std::string path);
	
	glm::mat4 m_ModelMatrix;
	
	std::string m_Directory;


	
private:
	GLuint m_VertexBuffer;
	GLuint m_TextureCoordBuffer;
	GLuint m_NormalBuffer;

	struct FaceDefinition
	{
		unsigned short VertexIndex;
		unsigned short TextureCoordIndex;
		unsigned short NormalIndex;
	};

	struct Face
	{
		std::vector<FaceDefinition> Definitions;
	};

	std::vector<Face> m_Faces;

	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec2> m_TextureCoords;
	std::vector<glm::vec3> m_Normals;

	
	
	std::string m_TexturePath;
	std::string m_MtlPath;

	bool Model::LoadTexture();
};

