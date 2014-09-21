#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>
#include "context.hh"
#include "font.hh"
#include "log.hh"

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
				al_init_font_addon();	// always success
				assert(al_init_ttf_addon(), "allegro_ttf init failed");
				assert(al_install_mouse(), "mouse init failed");
				assert(al_install_keyboard(), "keyboard init failed");
				library::log() << "allegro initialized\n";
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
		library::log() << "context(): created\n";
	}

	void context::render(command_list_cref cmds)
	{
		for (auto& cmd: cmds) {
			clip(cmd.clip);
			switch (cmd.kind) {
				case command::kind_type::rect:
					//clear(cmd.color);	// FIXME: don't blend
					al_draw_filled_rectangle(
							cmd.clip[0]+0.5f,
							cmd.clip[1]+0.5f,
							cmd.clip[2]+0.5f,
							cmd.clip[3]+0.5f,
							cmd.color);
					library::log() << "rect: "
						<< cmd.clip[0] << ", "
						<< cmd.clip[1] << ", "
						<< cmd.clip[2] << ", "
						<< cmd.clip[3] << "\n";
					break;
				case command::kind_type::text:
					draw_text(cmd.region, cmd.color, cmd.ch);
					library::log() << "text: "
						<< cmd.clip[0] << ", "
						<< cmd.clip[1] << ", "
						<< cmd.clip[2] << ", "
						<< cmd.clip[3] << "    "
						<< cmd.ch << "\n";
					break;
			}
		}
		al_flip_display();
	}

	void context::update(imgui::state& s)
	{
		al_wait_for_event(evq, nullptr);
		for (ALLEGRO_EVENT ev; al_get_next_event(evq, &ev); )
			switch (ev.type) {
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;

				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					s.mouse.down = true;
					break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
					s.mouse.down = false;
					break;
				case ALLEGRO_EVENT_MOUSE_AXES:
					s.mouse.x = ev.mouse.x;
					s.mouse.y = ev.mouse.y;
					break;
			}
	}

	void context::clip(xywh const& r)
	{
		al_set_clipping_rectangle(r.x, r.y, r.w, r.h);
	}

	void context::clear(rgba const& color)
	{
		al_clear_to_color(color);
	}
}

