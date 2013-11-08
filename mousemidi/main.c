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
#include "seq.h"
#include <err.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
	if (argc != 2) err(1, "usage: %s mouse_device_file", argv[0]);

	seq_load("music.seq");

	mouse_init(argv[1], $(void, (MouseAction ma) {
		switch (ma) {
			case MOUSE_HIT_CORNER:		seq_play();					break;
			case MOUSE_LEAVE_CORNER:	seq_stop();					break;
			case MOUSE_LEFT_DOWN:		seq_reset();				break;
			case MOUSE_RIGHT_DOWN:		seq_load("music.seq");		break;
			default:												break;
		}
	}));

	jack_init(NULL);

	jack_start();
	mouse_mainloop();

	return 0;
}

