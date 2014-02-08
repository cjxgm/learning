// vim: noet ts=4 sw=4 sts=0
#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "dsp.h"




size_t srate;	// sample rate




static jack_client_t* jack;
static jack_port_t* port_wave_in ;
static jack_port_t* port_wave_out;
static jack_port_t* port_midi_in ;
static jack_port_t* port_midi_out;

#define ERROR(RET, FMT, ARGS...) ({ \
	fprintf(stderr, FMT "\n",## ARGS); \
	exit(RET); \
})

static void shutdown_cb(void* arg);
static int process_cb(jack_nframes_t nframe, void* arg);
static int srate_cb(jack_nframes_t nframe, void* arg);




int main()
{
	// connect to jack
	jack = jack_client_open("dsp", JackNoStartServer, NULL);
	if (!jack) ERROR(1, "unable to connect to jack");

	// register callbacks
	jack_on_shutdown(jack, &shutdown_cb, NULL);
	jack_set_process_callback(jack, &process_cb, NULL);
	jack_set_sample_rate_callback(jack, &srate_cb, NULL);

	// register ports
	#define PORT(VAR, TYPE, DIRECTION) \
		port_ ## VAR = jack_port_register(jack, #VAR, \
				JACK_DEFAULT_ ## TYPE ## _TYPE, \
				JackPortIs ## DIRECTION ## put, 0); \
		if (!(port_ ## VAR)) ERROR(2, "unable to register port %s", #VAR)

	PORT(wave_in , AUDIO,  In);
	PORT(wave_out, AUDIO, Out);
	PORT(midi_in ,  MIDI,  In);
	PORT(midi_out,  MIDI, Out);

	#undef PORT

	// activate jack
	if (jack_activate(jack)) ERROR(3, "unable to activate jack");

	// kill main thread, then this process won't exit
	pthread_exit(NULL);

	// never reach here
	return 0;
}




static void* midi_out;	// <-- XXX midi_out defined here.
static int process_cb(jack_nframes_t nframe, void* arg)
{
	void* wave_in  = jack_port_get_buffer(port_wave_in , nframe);
	void* wave_out = jack_port_get_buffer(port_wave_out, nframe);
	void* midi_in  = jack_port_get_buffer(port_midi_in , nframe);
	      midi_out = jack_port_get_buffer(port_midi_out, nframe);
	jack_midi_clear_buffer(midi_out);	// clear first

	// notify midi events
	jack_nframes_t nevent = jack_midi_get_event_count(midi_in);
	for (int i=0; i<nevent; i++) {
		jack_midi_event_t ev;
		jack_midi_event_get(&ev, midi_in, i);
		process_midi(ev.time, ev.buffer);
	}

	// process wave
	process_wave(wave_in, wave_out, nframe);

	return 0;
}

void midi_write(size_t frame, uint8_t ev[3])
{
	jack_midi_event_write(midi_out, frame, ev, 3);
}

static int srate_cb(jack_nframes_t nframe, void* arg)
{
	srate = nframe;
	return 0;
}

static void shutdown_cb(void* arg)
{
	exit(0);
}

