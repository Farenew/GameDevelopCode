#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Bengine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		// mit : map iterator
		// std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);
		// lookup the texture and see if it is in the map
		auto mit = _textureMap.find(texturePath);

		// check if it is not in the map
		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "loaded Texture\n";

			return newTexture;
		}

		// std::cout << "used cached Texture\n";

		return mit->second;
	}

}