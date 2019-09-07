#pragma once

#include <string>

#include <GL/glew.h>
#include "GLTexture.h"

namespace Bengine {

	class Sprite
	{
	public:

		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

	private:

		float _x;
		float _y;
		float _width;
		float _height;

		// same as unsigned int, but this is guaranteed to be 32bit. 
		GLuint _vboID;

		GLTexture _texture;
	};

}