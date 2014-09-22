#pragma once
#include <allegro5/allegro.h>
#include <vector>
#include "state.hh"
#include "manage.hh"
#include "command.hh"
#include "quad.hh"

namespace imgui
{
	struct context
	{
		using display_type     = ALLEGRO_DISPLAY;
		using event_queue_type = ALLEGRO_EVENT_QUEUE;
		using display_mptr     = managed_ptr<display_type>;
		using event_queue_mptr = managed_ptr<event_queue_type>;
		using command_list     = std::vector<command>;
		using command_list_cref= command_list const&;

		context(int w, int h);

		operator bool () const { return running; }

		void render(command_list_cref cmds);
		void update(imgui::state& s);
		void clip(xywh const& r);
		void clear(rgba const& color);

	private:
		    display_mptr disp;
		event_queue_mptr evq;
		bool running;
	};
}

