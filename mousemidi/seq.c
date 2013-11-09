// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "seq.h"
#include "midi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

typedef enum NoteType
{
	NOTE_END,
	NOTE_MUSICAL,
	NOTE_VOLUME,
}
NoteType;

typedef struct Note
{
	NoteType type;
	int rewind;
	int next;
	unsigned char value;
}
Note;

static Note* notes;
static Note* note_current;
static unsigned char volume;

void seq_load(const char* file)
{
	if (notes) {
		free(notes);
		notes = NULL;
	}

	FILE* fp = fopen(file, "r");
	if (!fp) err(1, "unable to opena %s", file);

	const int initial_cap = 8;
	int cap  = initial_cap;
	int size = 0;
	notes = malloc(initial_cap*sizeof(Note));

	void append(Note note)
	{
		notes[size++] = note;
		if (size == cap)
			notes = realloc(notes, (cap<<=1) * sizeof(Note));
	}

	char pitch_shift = 0;
	char s[5];
	int rewind = 0;

	void parse_note()
	{
		unsigned char note_to_midi()
		{
			static const char* note_names[] = {
				"C-", "C#",
				"D-", "D#",
				"E-",
				"F-", "F#",
				"G-", "G#",
				"A-", "A#",
				"B-",
			};
			for (int i=0; i<12; i++)
				if (!strncmp(note_names[i], s, 2))
					return i + 12*(s[2]-'0') + pitch_shift;
			err(1, "unknown note %s", s);
		}
		if (s[0] == 'S') pitch_shift = (s[1]-'0')*10 + s[2]-'0';
		else {
			Note n;
			n.next  = !!s[3];
			n.rewind= rewind;
			if (s[3]) rewind++;
			else      rewind = 0;
			switch (s[0]) {
				case 'V':
					n.type  = NOTE_VOLUME;
					n.value = ((s[1]-'0')*10 + s[2]-'0') * 127 / 99;
					break;
				default:
					n.type  = NOTE_MUSICAL;
					n.value = note_to_midi();
					break;
			}
			append(n);
		}
	}

	while (fscanf(fp, "%4s", s) != EOF)
		parse_note();
	append((Note){ NOTE_END });

	fclose(fp);

	seq_reset();
}

void seq_play()
{
	for (;; note_current++)
		switch (note_current->type) {
			case NOTE_END: err(1, "let's fuck your score's last note");
			case NOTE_VOLUME: volume = note_current->value; break;
			case NOTE_MUSICAL:
				midi_key_press(0, note_current->value, volume);
				if (!note_current->next) return;
				break;
		}
}

void seq_stop()
{
	Note* p = note_current - note_current->rewind;
	for (; p <= note_current; p++)
		if (p->type == NOTE_MUSICAL)
			midi_key_release(0, p->value);
	if ((++note_current)->type == NOTE_END)
		note_current = notes;
}

void seq_reset()
{
	note_current = notes;
	volume = 0x60;
	midi_panic();
}

