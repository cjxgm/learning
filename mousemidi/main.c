// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "jack.h"
#include "midi.h"
#include "mouse.h"
#include "util.h"
#include <err.h>


int main(int argc, char* argv[])
{
	if (argc != 2) err(1, "usage: %s mouse_device_file", argv[0]);

	mouse_init(argv[1], $(void, (MouseAction ma) {
	}));

	jack_init($(MidiEvent, () {
		return midi_empty();
	}));

	jack_start();
	mouse_mainloop();

	return 0;
}

