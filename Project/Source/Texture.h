#pragma once
#include "PrecompiledHeader.h"
#include "SOIL.h"

class Texture
{
public:
	Texture(std::string path);

	~Texture();

	GLuint texture;

	void Load(std::string path);
	void Bind();
};
