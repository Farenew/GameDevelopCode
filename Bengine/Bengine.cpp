#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Bengine.h"

namespace Bengine {

	int init() {
		// initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// double buffer, means using two windows, one is using, one is cleared, makes game smooth. 
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}