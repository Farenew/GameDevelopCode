#pragma once

#include <GL/glew.h>

namespace Bengine {

	struct Position {
		float x;
		float y;
	};

	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		Color(GLubyte r = 255, GLubyte g = 255, GLubyte b = 255, GLubyte a = 255) : r{ r }, g{ g }, b{ b }, a{ a }{}

		Color& setColor(GLubyte R, GLubyte G, GLubyte B, GLubyte A) {
			r = R;
			g = G;
			b = B;
			a = A;
			return *this;
		}
	};


	struct UV {
		float U;
		float V;
	};


	struct Vertex {
		Position position;

		Color color;

		UV uv;

		void setPoistion(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.U = u;
			uv.V = v;
		}
	};

}