#pragma once

#include "TextureCache.h"
#include <string>

namespace Bengine {

	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

		static GLuint boundTexture;

	private:
		static TextureCache _textureCache;
	};

}