#include <allegro5/allegro_primitives.h>
#include "context.hh"

namespace imgui
{
	namespace
	{
		struct allegro
		{
			allegro()
			{
				assert(al_init(), "allegro init failed");
				assert(al_init_primitives_addon(), "allegro_primitives init failed");
				assert(al_install_mouse(), "mouse init failed");
				assert(al_install_keyboard(), "keyboard init failed");
			}
		};
		allegro _;
	}




	context::context(int w, int h)
	{
		#define MANAGE(WHAT, ...) \
				manage( \
					al_create_##WHAT (__VA_ARGS__), \
					&al_destroy_##WHAT, \
					#WHAT " creation failed")

		disp.MANAGE(display, w, h);
		 evq.MANAGE(event_queue);

		#undef MANAGE

		al_register_event_source(evq, al_get_display_event_source(disp));
		al_register_event_source(evq, al_get_mouse_event_source());
		al_register_event_source(evq, al_get_keyboard_event_source());

		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

		running = true;
	}

	void context::render(command_list_cref cmds)
	{
		for (auto& cmd: cmds)
			switch (cmd.kind) {
				case command::kind_type::rect:
					al_draw_filled_rectangle(
							cmd.xyxy[0]+0.5,
							cmd.xyxy[1]+0.5,
							cmd.xyxy[2]+0.5,
							cmd.xyxy[3]+0.5,
							al_map_rgba(
								cmd.rgba[0],
								cmd.rgba[1],
								cmd.rgba[2],
								cmd.rgba[3]));
					break;
				case command::kind_type::text:
					// TODO
					al_draw_filled_rectangle(
							cmd.xyxy[0]+0.5,
							cmd.xyxy[1]+0.5,
							cmd.xyxy[2]+0.5,
							cmd.xyxy[3]+0.5,
							al_map_rgba(255, 0, 0, 255));
					break;
			}
		al_flip_display();
	}

	void context::update(imgui::state& s)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(evq, &ev);
		switch (ev.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
		}
	}
}

