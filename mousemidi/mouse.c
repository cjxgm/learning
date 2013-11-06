// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "mouse.h"
#include <stdio.h>
#include <math.h>
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
static MouseEvent e;

static int is_at_corner;
static double x;
static double y;





void mouse_init(const char * mouse_file, MouseActionChangeFunc mac)
{
	fp = fopen(mouse_file, "r");
	if (!fp) err(1, "unable to open device '%s'", mouse_file);
	if (setvbuf(fp, NULL, _IONBF, 0))
		err(1, "unable to set unbuffered mode on '%s'", mouse_file);

	mouse_action_change = mac;
}


void mouse_next_frame()
{
	x += e.x/20.0;
	y += e.y/20.0;
	e.x = e.y = 0;
	double sqrx = x*x;
	double sqry = y*y;
	double sqrd = sqrx + sqry;	// d for distance
	double d = sqrt(sqrd);
	if (d > 0.9) {
		// normalize when needed
		if (d > 1.0) {
			x /= d;
			y /= d;
		}

		if (is_at_corner)
			mouse_action_change(MOUSE_MOVE_ON_CORNER);
		else {
			is_at_corner = 1;
			mouse_action_change(MOUSE_HIT_CORNER);
			printf("+\n");
		}
	}
	else if (is_at_corner) {
		is_at_corner = 0;
		mouse_action_change(MOUSE_LEAVE_CORNER);
		printf("-\n");
	}
	printf("\r %c %5.2f %5.2f -> %4.2f ", (is_at_corner ? '*' : ' '), x, y, sqrt(x*x+y*y));
	fflush(stdout);

	// fall back to center automatically
	x *= 0.95;
	y *= 0.95;
	if (fabs(x) < 0.1) x = 0.0;
	if (fabs(y) < 0.1) y = 0.0;
}


void mouse_mainloop()
{
	while (1) {
		fread(&e, sizeof(e), 1, fp);
		e.y = -e.y;	// remember? it's flipped!
	}
}

