#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

// like the "auto" keyword in c++11
#define auto(var,expr) typeof(expr) (var) = (expr)
#define ANY (void *)

int main()
{
	auto(conn, xcb_connect(NULL, NULL));
	auto(scr , xcb_setup_roots_iterator(xcb_get_setup(conn)).data);
	auto(root, scr->root);

	// mask the key event
	const static uint32_t value = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
	xcb_change_window_attributes(conn, root, XCB_CW_EVENT_MASK, &value);

	xcb_grab_key(conn, 1, root, XCB_MOD_MASK_ANY, 'a', XCB_GRAB_MODE_SYNC, XCB_GRAB_MODE_SYNC);
	while (1) {
		printf("waiting...");
		fflush(stdout);
		auto(e, xcb_wait_for_event(conn));
		printf("got!\n");
		switch (e->response_type & ~0x80) {
			case XCB_KEY_PRESS: {
				xcb_button_press_event_t * ev = ANY e;
				printf("key press\n");
			} break;
			case XCB_KEY_RELEASE: {
				xcb_button_press_event_t * ev = ANY e;
				printf("key release\n");
			} break;
			default: {
				printf("unknown event\n");
			} break;
		}
		free(e);
	}

	printf("screen resolution: %dx%d\n", scr->width_in_pixels, scr->height_in_pixels);
	xcb_disconnect(conn);
	return 0;
}

