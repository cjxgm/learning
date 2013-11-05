// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <err.h>


#define $(ret, body) ({ ret __anonymous__ body (void*)&__anonymous__; })



static jack_client_t	* jack;
static jack_port_t		* ports_in [2];
static jack_port_t		* ports_out[2];


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

MidiEvent to_be_played;



void jack_init()
{
	// connect to jack
	if (!(jack = jack_client_open("jack-midi", JackNoStartServer, NULL)))
		err(1, "unable to connect to jack");

	// register callbacks
	jack_on_shutdown(jack, $(void, () {
		exit(0);
	}), NULL);

	/*
	jack_set_buffer_size_callback(jack,
		$(int, (jack_nframes_t nframe) {
			// this will be called once jack activated.
			return 0;
		}), NULL);
	*/

	jack_set_process_callback(jack, $(int, (jack_nframes_t nframe) {
		void*  in = jack_port_get_buffer(ports_in [0], nframe);
		void* out = jack_port_get_buffer(ports_out[0], nframe);
		jack_midi_clear_buffer(out);
		jack_nframes_t nevent = jack_midi_get_event_count(in);
		for (int i=0; i<nevent; i++) {
			jack_midi_event_t ev;
			jack_midi_event_get(&ev, in, i);
			printf("%d: %.2X %.2X %.2X\n", ev.time, ev.buffer[0], ev.buffer[1], ev.buffer[2]);
			jack_midi_event_write(out, ev.time, ev.buffer, 3);
			fflush(stdout);
		}
		if (midi_has_data(to_be_played)) {
			jack_midi_event_write(out, 0, to_be_played.buffer, 3);
			printf("played: %.2X %.2X %.2X\n",
					to_be_played.buffer[0],
					to_be_played.buffer[1],
					to_be_played.buffer[2]);
			midi_reset(&to_be_played);
		}
		jack_midi_event_write(out, 0, to_be_played.buffer, 3);
		return 0;
	}), NULL);

	// register ports
	ports_in[0] = jack_port_register(jack, "midi_in_0",
			JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	ports_in[1] = jack_port_register(jack, "playback_1",
			JACK_DEFAULT_AUDIO_TYPE,
			JackPortIsInput|JackPortIsPhysical|JackPortIsTerminal, 0);

	ports_out[0] = jack_port_register(jack, "midi_out_0",
			JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
	ports_out[1] = jack_port_register(jack, "capture_1",
			JACK_DEFAULT_AUDIO_TYPE,
			JackPortIsOutput|JackPortIsPhysical|JackPortIsTerminal, 0);

	// other setups
	//global_sample_rate = jack_get_sample_rate(jack);
}


void jack_start()
{
	if (jack_activate(jack)) err(1, "unable to activate jack");
}


void jack_close()
{
	if (jack) {
		jack_deactivate(jack);
		jack_client_close(jack);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2) err(1, "usage: %s mouse_device_file", argv[0]);

	// open mouse device and set unbuffered mode
	FILE * fp = fopen(argv[1], "r");
	if (!fp) err(1, "unable to open device '%s'", argv[1]);
	if (setvbuf(fp, NULL, _IONBF, 0))
		err(1, "unable to set unbuffered mode on '%s'", argv[1]);

	jack_init();
	jack_start();

	MouseEvent e;
	while (1) {
		fread(&e, sizeof(e), 1, fp);
		e.y = -e.y;	// remember? it's flipped!
		printf("%hhd %hhd\n", e.x, e.y);
		if (e.x > 10) to_be_played = midi_key_press(0, 80, 100);
		else to_be_played = midi_key_release(0, 80);
	}
}

