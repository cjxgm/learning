#pragma once
#include <SDL2/SDL.h>
#include <glm/vec4.hpp>
#include <memory>
#include <string>

namespace gl
{
	struct context
	{
		using raw_window_type = SDL_Window;
		using raw_window_sptr = std::shared_ptr<raw_window_type>;
		using raw_context_type = SDL_GLContext;
		using raw_context_ptr  = raw_context_type *;
		using raw_context_sptr = std::shared_ptr<raw_context_type>;
		using dimension_type = int;
		using title_type = std::string;
		using title_cref = title_type const&;

		context(dimension_type w=800, dimension_type h=600,
				title_cref title="opengl");

		void update(bool wait=false);
		void clear(glm::vec4 const& color={0, 0, 0, 1}) const;
		operator bool() const { return running; }

	private:
		raw_window_sptr win;
		raw_context_sptr ctx;
		bool running;
	};
}

