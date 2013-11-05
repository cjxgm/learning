// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "mouse.h"
#include <stdio.h>
#include <err.h>


typedef struct MouseEvent
{
	unsigned int l:1;	// is left     mouse button down?
	unsigned int r:1;	// is    right mouse button down?
	unsigned int m:1;	// is  middle  mouse button down?
	unsigned int  :5;	// I don't know what those value mean.
	char x;
	char y;	// note that this value is actually "-y", I mean, flipped.
}
__attribute__((__packed__))
MouseEvent;

static MouseActionChangeFunc mouse_action_change;
static FILE * fp;




void mouse_init(const char * mouse_file, MouseActionChangeFunc mac)
{
	fp = fopen(mouse_file, "r");
	if (!fp) err(1, "unable to open device '%s'", mouse_file);
	if (setvbuf(fp, NULL, _IONBF, 0))
		err(1, "unable to set unbuffered mode on '%s'", mouse_file);

	mouse_action_change = mac;
}


void mouse_mainloop()
{
	MouseEvent e;
	while (1) {
		fread(&e, sizeof(e), 1, fp);
		e.y = -e.y;	// remember? it's flipped!
		printf("%hhd %hhd\n", e.x, e.y);
		mouse_action_change(MOUSE_LEAVE_CORNER);
	}
}

