SinOsc s => NRev r => dac;
.5 => s.gain;
.1 => r.mix;

while (true) {
	Math.random2f(30, 1000) => s.freq;
	10::ms => now;
}
