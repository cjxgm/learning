<<< "Assignment 1: Beatbox" >>>;

SinOsc s => dac;	// the drum Sound
TriOsc m => dac;	// the Music sound
440 => float f;		// will be decreased to 0 to make a drum sound
0   => int beat;	// one drum beat  (from 0 to 3)
0   => int section; //   4 drum beats (from 0 to 7)
0   => int part;	//   8 sections form one part
now => time begin;	// to calculate duration

while (part < 8) {
	0 => m.gain;
	0 => m.freq;
	// music pattern
	if (section >= 6 && (beat==0 || beat==2)) {
		if (f > 100) 0.3 => m.gain;
		else 0 => m.gain;
		if (part==7 && section==7) 480 - (1-beat/2)*60 => m.freq;
		else 480 - beat/2*60 => m.freq;
	}
	if (part==1 || part==4 || part==5) {
		if ((section==0 || section==2 || section==4) && beat!=0) {
			300 + 60*beat => m.freq;
			0.1 + (beat-1)*0.2 => m.gain;
		}
	}
	if (part == 2) {
		0.24 => m.gain;
		if (section/2 == 0) 480 => m.freq;
		if (section/2 == 1) 420 => m.freq;
		if (section/2 == 2) 380 => m.freq;
		if (section/2 == 3) 320 => m.freq;
	}
	if (part == 3) {
		0.24 => m.gain;
		if (section/4 == 0) 360 => m.freq;
	}


	// drum pattern
	if (section != 6) {
		if (beat == 0) 1 => s.gain;	// tah!
		else 0.3 => s.gain;			// snare?
	}
	else {	// a little bit variation when section == 6
		if (beat < 3) 1 => s.gain;	// triple the tah!
		else 0.3 => s.gain;			// snare?
	}

	// drop the frequency to make a drum sound
	f-0.1 => f;
	if (section == 7 && beat == 0) 560 * (f/440) => s.freq;	// make it a little higher for variance
	else f => s.freq;


	// reset f; increase beat, section and part when needed.
	if (f <= 0) {
		440 => f;
		beat++;		if (beat == 4) 0 => beat;
		if (beat == 0) {
			section++;
			if (section == 8) {
				0 => section;
				part++;
				<<< "part", part, "begins when", (now-begin)/second, "seconds passed." >>>;
			}
		}
	}

	// exactly 30 seconds for:
	//		4400 loops per beat
	//		   4 beats per section
	//		8 sections per part
	//         8 parts per song  
	(30.0/8/8/4/4400)::second => now;
}

<<< "song ended with duration", (now-begin)/second >>>;
