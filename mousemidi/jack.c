// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "jack.h"
#include "util.h"
#include "midi.h"
#include "mouse.h"
#include <stdio.h>
#include <stdlib.h>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <err.h>





static jack_client_t	* jack;
static jack_port_t		* ports_in [2];
static jack_port_t		* ports_out[2];
static NextFrameCb		  next_frame_cb;


void jack_init(NextFrameCb nfc)
{
	// connect to jack
	if (!(jack = jack_client_open("mousemidi", JackNoStartServer, NULL)))
		err(1, "unable to connect to jack");

	// register callbacks
	jack_on_shutdown(jack, $(void, () {
		exit(0);
	}), NULL);

	jack_set_process_callback(jack, $(int, (jack_nframes_t nframe) {
		mouse_next_frame();
		if (next_frame_cb) next_frame_cb();

		void*  in = jack_port_get_buffer(ports_in [0], nframe);
		void* out = jack_port_get_buffer(ports_out[0], nframe);
		jack_midi_clear_buffer(out);

		jack_nframes_t nevent = jack_midi_get_event_count(in);
		for (int i=0; i<nevent; i++) {
			jack_midi_event_t ev;
			jack_midi_event_get(&ev, in, i);
			printf("%d: %.2X %.2X %.2X\n", ev.time,
					ev.buffer[0], ev.buffer[1], ev.buffer[2]);
			jack_midi_event_write(out, ev.time, ev.buffer, 3);
			fflush(stdout);
		}

		while (midi_count()) {
			MidiEvent me = midi_pop();
			jack_midi_event_write(out, 0, me.buffer, 3);
			printf(">>> %.2X %.2X %.2X\n",
					me.buffer[0], me.buffer[1], me.buffer[2]);
		}

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

	// save callback(s)
	next_frame_cb = nfc;
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

