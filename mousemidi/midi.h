// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#pragma once

#include <jack/jack.h>
#include <jack/midiport.h>


typedef struct MidiEvent
{
	jack_midi_data_t buffer[3];
}
MidiEvent;


static inline MidiEvent midi_key_press(
		jack_midi_data_t channel,
		jack_midi_data_t note,
		jack_midi_data_t velocity)
{
	return (MidiEvent){{ 0x90+channel, note, velocity }};
}

static inline MidiEvent midi_key_release(
		jack_midi_data_t channel,
		jack_midi_data_t note)
{
	return (MidiEvent){{ 0x80+channel, note, 0 }};
}

static inline MidiEvent midi_panic()
{
	return (MidiEvent){{ 0xb0, 0x7b, 0 }};
}

static inline int midi_has_data(MidiEvent ev)
{
	return !!ev.buffer[0];
}

static inline void midi_reset(MidiEvent* ev)
{
	ev->buffer[0] = ev->buffer[1] = ev->buffer[2];
}

