// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#pragma once

typedef enum MouseAction
{
	MOUSE_HIT_CORNER,
	MOUSE_LEAVE_CORNER,
	MOUSE_MOVE_ON_CORNER,
	MOUSE_LEFT_DOWN,
	MOUSE_RIGHT_DOWN,
	MOUSE_MIDDLE_DOWN,
}
MouseAction;

typedef void (*MouseActionChangeFunc)(MouseAction ma);

void mouse_init(const char * mouse_file, MouseActionChangeFunc mac);
void mouse_next_frame();
void mouse_mainloop();

