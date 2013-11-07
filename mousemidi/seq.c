// vim: noet ts=4 sw=4 sts=0

/* Licensed under the MIT License.
 *
 * Contributers:
 * 		eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>
 */

#include "seq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


unsigned char* seq_load(const char* file)
{
	FILE* fp = fopen(file, "r");
	if (!fp) err(1, "unable to opena %s", file);

	const int initial_cap = 8;
	int cap  = initial_cap;
	int size = 0;
	unsigned char* notes = malloc(initial_cap);

	void append(unsigned char note)
	{
		notes[size++] = note;
		if (size == cap)
			notes = realloc(notes, cap<<=1);
	}

	char pitch_shift = 0;
	char s[4];

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
		else append(note_to_midi());
	}

	while (fscanf(fp, "%3s", s) != EOF)
		parse_note();
	append(0);

	fclose(fp);

	return notes;
}

