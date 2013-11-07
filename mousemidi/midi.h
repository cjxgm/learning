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


// midi event queue management
void midi_push(MidiEvent me);
MidiEvent midi_pop();
int midi_count();




// common events pushing helpers
static inline void midi_push_literal(
		jack_midi_data_t a,
		jack_midi_data_t b,
		jack_midi_data_t c)
{
	midi_push((MidiEvent){{ a, b, c }});
}

static inline void midi_key_press(
		jack_midi_data_t channel,
		jack_midi_data_t note,
		jack_midi_data_t velocity)
{
	midi_push_literal(0x90+channel, note, velocity);
}

static inline void midi_key_release(
		jack_midi_data_t channel,
		jack_midi_data_t note)
{
	midi_push_literal(0x80+channel, note, 0);
}

static inline void midi_panic()
{
	midi_push_literal(0xb0, 0x7b, 0);
}

