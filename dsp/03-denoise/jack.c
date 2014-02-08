// vim: noet ts=4 sw=4 sts=0
#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "dsp.h"

static jack_client_t* jack;
static jack_port_t* port_wave_in ;
static jack_port_t* port_wave_out;
static jack_port_t* port_midi_in ;
static jack_port_t* port_midi_out;

#define ERROR(RET, ARGS...) ({ fprintf(stderr, ARGS); exit(RET); })

static void shutdown_cb(void* arg);
static int process_cb(jack_nframes_t nframe, void* arg);

int main()
{
	// connect to jack
	jack = jack_client_open("dsp", JackNoStartServer, NULL);
	if (!jack) ERROR(1, "unable to connect to jack");

	// register callbacks
	jack_on_shutdown(jack, &shutdown_cb, NULL);
	jack_set_process_callback(jack, &process_cb, NULL);

	// register ports
	#define PORT(NAME, TYPE, DIRECTION) \
		jack_port_register(jack, (NAME), \
				JACK_DEFAULT_ ## TYPE ## _TYPE, \
				JackPortIs ## DIRECTION ## put, 0)

	port_wave_in  = PORT( "input", AUDIO, In );
	port_wave_out = PORT("output", AUDIO, Out);
	port_midi_in  = PORT( "input",  MIDI, In );
	port_midi_out = PORT("output",  MIDI, Out);

	// activate jack
	if (jack_activate(jack)) ERROR(2, "unable to activate jack");

	// kill main thread, then this process won't exit
	pthread_exit(NULL);

	// never reach here
	return 0;
}

static void shutdown_cb(void* arg)
{
	exit(0);
}

static int process_cb(jack_nframes_t nframe, void* arg)
{
	void* wave_in  = jack_port_get_buffer(port_wave_in , nframe);
	void* wave_out = jack_port_get_buffer(port_wave_out, nframe);
	void* midi_in  = jack_port_get_buffer(port_midi_in , nframe);
	void* midi_out = jack_port_get_buffer(port_midi_out, nframe);
	jack_midi_clear_buffer(midi_out);

	process_wave(wave_in, wave_out, nframe);
	process_midi(midi_in, midi_out, nframe);
/*
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
*/
	return 0;
}

