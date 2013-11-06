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

	static unsigned char notes[] = { 0x32, 0x37, 0x3b, 0x37, 0x3e, 0x37, 0x3b, 0x37 };
	int note_pos = 0;
	int note_status = 0;

	mouse_init(argv[1], $(void, (MouseAction ma) {
		if (ma == MOUSE_HIT_CORNER) note_status++;
		if (ma == MOUSE_LEAVE_CORNER) note_status++;
	}));

	jack_init($(MidiEvent, () {
		MidiEvent me = midi_empty();
		switch (note_status) {
			case 1:
				me = midi_key_press(0, notes[note_pos], 100);
				note_status++;
				break;
			case 3:
				me = midi_key_release(0, notes[note_pos++]);
				note_pos %= sizeof(notes) / sizeof(*notes);
				note_status = 0;
				break;
		}
		return me;
	}));

	jack_start();
	mouse_mainloop();

	return 0;
}

