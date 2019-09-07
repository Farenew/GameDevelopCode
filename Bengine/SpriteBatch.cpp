#include "SpriteBatch.h"

#include <algorithm>

namespace Bengine {

	// initialize vbo and vao to 0 to make sure they have no attached buffers
	SpriteBatch::SpriteBatch(): _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertics);
		}
		glBindVertexArray(0);
	}

	// for each glyph, add it to renderbatch, if glyph gets a new texture, make a new batch
	void SpriteBatch::createRenderBatch()
	{
		std::vector<Vertex> vertices;

		if (_glyphs.empty())
			return;
		
		int offset = 0;

		//// two triangles, 6 dots
		// RenderBatch myBatch(0, 6, _glyphs[0]->texture);
		// using emplace_back can do creation, and then push back
		// pass in a render batch object
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);

		vertices.push_back(_glyphs[0]->topLeft);
		vertices.push_back(_glyphs[0]->bottomLeft);
		vertices.push_back(_glyphs[0]->bottomRight);
		vertices.push_back(_glyphs[0]->bottomRight);
		vertices.push_back(_glyphs[0]->topRight);
		vertices.push_back(_glyphs[0]->topLeft);
		// anytime make 6 new vertices, we add 6 offset
		offset += 6;

		// cg, current glyph, strat from 1, beacuse we just did 0
		for (int cg = 1; cg < _glyphs.size(); cg++) {
			// only if the current texture is different from the previous one, we add new glyph
			// or we just add the number of vertices
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else {
				_renderBatches.back().numVertics += 6;
			}

			vertices.push_back(_glyphs[cg]->topLeft);
			vertices.push_back(_glyphs[cg]->bottomLeft);
			vertices.push_back(_glyphs[cg]->bottomRight);
			vertices.push_back(_glyphs[cg]->bottomRight);
			vertices.push_back(_glyphs[cg]->topRight);
			vertices.push_back(_glyphs[cg]->topLeft);
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		// orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// upload the data, upload a certain range of data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{

		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		// bind means anything you did to change the OpenGL 
		// state of object would be stored to vertex array object
		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);


		// in attribute array, 0 means position
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		// this is the position attribute pointer
		// tell GL want to draw, 0 means attri array, 2 means x and y, GL_float type variable
		// GL_FALSE not normalized, stride is 0, pointer means offset
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));


		// this is going to disable all vertex attri array
		glBindVertexArray(0);
	}

	// set up any state we need to bendering
	void SpriteBatch::bufferClear(GlyphSortType sortType /*= GlyphSortType::TEXTURE */)
	{
		_sortType = sortType;
		_renderBatches.clear();
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();

	}

	// in end, start all sorting
	void SpriteBatch::bufferUpdate()
	{
		sortGlyphs();
		createRenderBatch();
	}

	// add a glyph to a vector of glyphs
	void SpriteBatch::addGlyph(const glm::vec4& desRectange, const glm::vec4& uvRectangle, GLuint texture, float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph;

		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		// Rectangle[3] = height
		newGlyph->topLeft.setPoistion(desRectange.x, desRectange.y + desRectange[3]);
		newGlyph->topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle[3]);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPoistion(desRectange.x, desRectange.y);
		newGlyph->bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

		// Rectangle[2] = width
		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPoistion(desRectange.x + desRectange[2], desRectange.y);
		newGlyph->bottomRight.setUV(uvRectangle.x + uvRectangle[2], uvRectangle.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPoistion(desRectange.x + desRectange[2], desRectange.y + desRectange[3]);
		newGlyph->topRight.setUV(uvRectangle.x + uvRectangle[2], uvRectangle.y + uvRectangle[3]);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::sortGlyphs()
	{
		// std::stable_sort
		switch (_sortType)
		{
		case GlyphSortType::NONE:
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		default:
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}