#include "context.hh"
#include "../log.hh"
#include <GL/glew.h>
#include <stdexcept>

namespace {
	struct sdl
	{
		sdl()
		{
			library::log() << "sdl::(ctor)\n";
			SDL_Init(SDL_INIT_EVERYTHING);
			library::log() << "\teverything initialized.\n";
		}
		~sdl()
		{
			library::log() << "sdl::(dtor)\n";
			SDL_Quit();
			library::log() << "\tquit.\n";
		}
	};

	static sdl _;
}

namespace gl
{
	context::context(dimension_type w, dimension_type h, title_cref title)
	{
		library::log() << "context::(ctor)\n";
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

		library::log() << "\tattributes set.\n";
		win = {
			SDL_CreateWindow(title.c_str(),
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					w, h,
					SDL_WINDOW_OPENGL),
			&SDL_DestroyWindow
		};
		library::log() << "\topengl window created.\n";

		ctx = {
			new raw_context_type(SDL_GL_CreateContext(win.get())),
			[](raw_context_ptr ctx) {
				SDL_GL_DeleteContext(*ctx);
				delete ctx;
			}
		};
		library::log() << "\topengl context created.\n";

		if (glewInit() != GLEW_OK)
			throw std::runtime_error{"glew initialization failed."};

		running = true;
	}

	void context::update(bool wait)
	{
		SDL_GL_SwapWindow(win.get());

		if (wait) SDL_WaitEvent(nullptr);

		SDL_Event e;
		while (SDL_PollEvent(&e))
			switch (e.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
	}

	void context::clear(glm::vec4 const& color)
	{
		static glm::vec4 clear_color;
		if (clear_color != color) {
			clear_color = color;
			glClearColor(
					clear_color[0],
					clear_color[1],
					clear_color[2],
					clear_color[3]);
			library::log() << "context::clear(): color changed.\n";
		}
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

