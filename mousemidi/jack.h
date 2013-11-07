// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#pragma once

#include "midi.h"

typedef void (*NextFrameCb)();

void jack_init(NextFrameCb next_frame_cb);
void jack_start();
void jack_close();

