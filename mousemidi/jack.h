// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#pragma once

#include "midi.h"

typedef MidiEvent (*GetMidiEventFunc)();

void jack_init(GetMidiEventFunc get_midi_event);
void jack_start();
void jack_close();

