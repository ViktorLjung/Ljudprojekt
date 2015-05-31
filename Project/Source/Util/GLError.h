#ifndef GLError_h__
#define GLError_h__

#include "PrecompiledHeader.h"
#include <iostream>

inline bool _GLERROR(char* info, char* file, char* func, unsigned int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		_LOG(LOG_LEVEL_ERROR, file, func, line, "GL Error: %s %i %s", info, error, gluErrorString(error));
		return true;
	}

	return false;
}

#define GLERROR(function) \
	_GLERROR(function, __BASE_FILE__, __func__, __LINE__)

#endif // GLError_h__