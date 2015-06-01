#include "Model.h"

Model::Model(std::string path)
{
	LoadOBJ(path);
	CreateBuffers();

	m_ModelMatrix = glm::mat4();
}

glm::mat4 Model::ModelMatrix()
{
	return m_ModelMatrix;
}

void Model::ModelMatrix(glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	m_ModelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(orientation) * glm::scale(scale);
}

void Model::ModelMatrix(glm::mat4 trans)
{
	m_ModelMatrix = trans;
}



bool Model::LoadTexture()
{
	std::ifstream file(m_MtlPath);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open .mtl \"%s\": %s", m_MtlPath, strerror(errno));
		return false;
	}

	LOG_INFO("Parsing .mtl %s", m_MtlPath);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0)
			continue;

		std::stringstream ss(line);

		std::string prefix;
		ss >> prefix;

		// Ignore comments
		if (prefix == "#")
			continue;

		if (prefix == "map_Kd")
		{
			std::string textureFileName;
			ss >> textureFileName;

			m_TexturePath = m_Directory;
			m_TexturePath += "/";
			m_TexturePath += textureFileName;
		}
	}

	return true;
}



bool Model::LoadOBJ(std::string path)
{
	m_Directory = path;

	std::vector<std::tuple<float, float, float>> Vertices;
	std::vector<std::tuple<float, float, float>> Normals;
	std::vector<std::tuple<float, float>> TextureCoords;

	std::ifstream file(path);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open .obj \"%s\": %s", path, strerror(errno));
		return false;
	}

	LOG_INFO("Parsing .obj %s", path);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0)
			continue;

		std::stringstream ss(line);

		std::string prefix;
		ss >> prefix;

		// Ignore comments
		if (prefix == "#")
			continue;

		// Material statement
		if (prefix == "mtllib")
		{
			//Getting directory
			size_t found;
			found = m_Directory.find_last_of("/\\");
			m_Directory = m_Directory.substr(0, found);

			std::string mtlFileName;
			ss >> mtlFileName;

			m_MtlPath = m_Directory;
			m_MtlPath += "/";
			m_MtlPath += mtlFileName;

			continue;
		}

		// Vertices
		if (prefix == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			Vertices.push_back(std::make_tuple(x, y, z));
			continue;
		}

		// Normals
		if (prefix == "vn")
		{
			float x, y, z;
			ss >> x >> y >> z;
			Normals.push_back(std::make_tuple(x, y, z));
		}

		// Texture coordinates
		if (prefix == "vt")
		{
			float u, v;
			ss >> u >> v;
			TextureCoords.push_back(std::make_tuple(u, v));
		}

		// Face definitions
		if (prefix == "f")
		{
			Face face;

			std::string faceDefString;
			while (ss >> faceDefString)
			{
				std::stringstream ss2(faceDefString);
				FaceDefinition faceDef = { 0, 0, 0 };

				ss2 >> faceDef.VertexIndex;
				ss2.ignore(); // Ignore first delimiter
				if (!ss2)
					continue;

				if (ss2.peek() == '/')
				{
					ss2.ignore();
					ss2 >> faceDef.NormalIndex;
				}
				else
				{
					ss2 >> faceDef.TextureCoordIndex;
					ss2.ignore();
					ss2 >> faceDef.NormalIndex;
				}
				face.Definitions.push_back(faceDef);
			}
			m_Faces.push_back(face);
		}
	}
	

	for (auto face : m_Faces)
	{
		// Face definitions
		for (auto faceDef : face.Definitions)
		{
			glm::vec3 vertex;
			std::tie(vertex.x, vertex.y, vertex.z) = Vertices[faceDef.VertexIndex - 1];
			m_Vertices.push_back(vertex);

			if (faceDef.NormalIndex != 0)
			{
				glm::vec3 normal;
				std::tie(normal.x, normal.y, normal.z) = Normals[faceDef.NormalIndex - 1];
				m_Normals.push_back(normal);
			}

			if (faceDef.TextureCoordIndex != 0)
			{
				glm::vec2 texCoord;
				std::tie(texCoord.x, texCoord.y) = TextureCoords[faceDef.TextureCoordIndex - 1];
				m_TextureCoords.push_back(texCoord);
			}
		}
	}
	
	LoadTexture();
	return true;
}


void Model::CreateBuffers()
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

	m_Texture = SOIL_load_OGL_texture(m_TexturePath.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

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

void Model::Bind()
{
	glBindVertexArray(m_VertexArrayObject);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

int Model::NumberOfVertices()
{
	return m_Vertices.size();
}


