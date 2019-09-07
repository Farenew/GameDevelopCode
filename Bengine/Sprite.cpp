#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace Bengine {

	Sprite::Sprite() {
		// initial value
		_vboID = 0;
	}

	Sprite::~Sprite() {
		if (_vboID != 0) {
			// free buffer, 1 means 1 buffer. 
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		// if this is 0 means it is not generated
		if (_vboID == 0) {
			// 1 stands for 1 buffer, vertex buffer id will always be different
			glGenBuffers(1, &_vboID);
		}


		Vertex vertexData[6];
		// first triangle
		vertexData[0].setPoistion(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPoistion(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPoistion(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		// second triangle
		vertexData[3].setPoistion(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPoistion(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPoistion(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);


		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);

		// bind the buffer, means we want this buffer to be active, bind it to GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		// upload the buffer data, vertexData is start pointer of data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// unbind the buffer, buffer id will always be positive, 0 means no buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void Sprite::draw() {

		glBindTexture(GL_TEXTURE_2D, _texture.id);

		// bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);



		// means draw triangles. not using GL_QUADS means some computer may not support quad. start from 0, draw 6 vertex.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}