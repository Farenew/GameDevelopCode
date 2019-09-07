#include "Window.h"
#include "Errors.h"

namespace Bengine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}


	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		//if (currentFlags & NORMAL)
		//	flags |= SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;

		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		// create a openGL window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (_sdlWindow == nullptr) {
			fatalError("SDL window could not be created!");
		}

		// create openGL context
		SDL_GLContext glContex = SDL_GL_CreateContext(_sdlWindow);
		if (glContex == nullptr) {
			fatalError("SDL_GLContext could not be created!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize GLEW!");
		}

		// print current openGL version
		std::printf("***    OpenGL Version: %s    ***\n\n\n", glGetString(GL_VERSION));
		// set background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		// set VSYNC, if it is 0, then not setting VSYNC
		SDL_GL_SetSwapInterval(1);

		// enable alpha to make graph transparent
		glEnable(GL_BLEND);
		// blend mode, first factor * sec factor. here is alpha * 1-src_alpha.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		// swap window buffer
		SDL_GL_SwapWindow(_sdlWindow);
	}

}