#pragma once

#include <GL\glew.h>
#include "Vertex.h"
#include <glm\glm.hpp>

#include <vector>

namespace Bengine {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	// used to hold vertices, texture, depth
	struct Glyph {

		GLuint texture;

		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertics, GLuint Texture) :
			offset(Offset),
			numVertics(NumVertics),
			texture(Texture)
		{

		}
		GLuint offset;
		// number of vertics need to draw
		GLuint numVertics;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		// give it a defalut parameter
		void bufferClear(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void bufferUpdate();

		// glm::vec2 position, glm::vec2 dimensions combine them as vec4. so (desRect[0], desRect[1]) = (desRect.x, desRect.y) = (x,y)
		// (desRect[2], desRect[3]) = (desRect.w, desRect.z) = (width,height), here we will treat Z component as width and W component as height
		void addGlyph(const glm::vec4& desRectange, const glm::vec4& uvRectangle, GLuint texture, float depth, const Color& color);

		void renderBatch();

	private:
		void createRenderBatch();
		void createVertexArray();
		void sortGlyphs();


		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		// Vertex Array Object
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;
	};

}