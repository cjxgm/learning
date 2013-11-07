// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "midi.h"
#include "util.h"
#include <stdio.h>
#include <err.h>
#include <pthread.h>

static MidiEvent events[1024];
static int nevent = 0;
static MidiEvent* head = events;
static MidiEvent* tail = events;

static void locker(int is_lock)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	if (is_lock) pthread_mutex_lock(&mutex);
	else       pthread_mutex_unlock(&mutex);
}

void midi_push(MidiEvent me)
{
	if (midi_count() == LEN(events)) {
		fprintf(stderr, "midi queue overflow, ignoring %.2x %.2x %.2x\n",
				me.buffer[0], me.buffer[1], me.buffer[2]);
		return;
	}

	locker(1);
	*tail++ = me;
	if (tail == &events[LEN(events)]) tail = events;
	nevent++;
	locker(0);
}

MidiEvent midi_pop()
{
	if (!midi_count()) err(1, "midi queue underflow");

	locker(1);
	MidiEvent me = *head++;
	if (head == &events[LEN(events)]) head = events;
	nevent--;
	locker(0);

	return me;
}

int midi_count()
{
	locker(1);
	int count = nevent;
	locker(0);
	return count;
}

