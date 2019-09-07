#include "ImageLoader.h"
#include "IOManager.h"
#include "picoPNG.h"
#include "Errors.h"

namespace Bengine {

	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture = {};
		// input data, get data from image
		std::vector<unsigned char> in;
		std::vector<unsigned char> out;
		unsigned long width, height;

		if (IOManager::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG to buffer!");
		}

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}

		/* para1: specifies the number of texture names to be generated.
		   para2: specifies an array in which the generated texture names are stored.*/
		glGenTextures(1, &(texture.id));

		// bind a named texture to a texturing target
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// upload to openGL texture, store to graphic RAM
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		// glTexParameteri sets the texture parameter
		// GL_TEXTURE_WRAP_S menas over the coordinate, GL_REPEAT we want to repeat it
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// how to generate different magnitude pictures, use linear interplation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		// unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}

}