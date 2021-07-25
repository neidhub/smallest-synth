import("stdfaust.lib");


freq = nentry("freq", 400, 0, 20000, 0.1);
//gain = nentry("gain", 0.5, 0, 1, 0.01);
gate = button("gate") : si.smoo;

// Teensy Oscillator
// lowering the resolution of the sine oscillators' lookup table 
// in order to make computation easier on the Teensy MCU and memory
// from https://github.com/makingsoundmachines/Faust-on-Teensy/blob/main/AdditiveSynth_Faust/faustAdditive.dsp

oscTeensy(f) = rdtable(tablesize, os.sinwaveform(tablesize), int(os.phasor(tablesize,f)))
with{
    tablesize = 1 << 13; // instead of 1 << 16
};


process = oscTeensy(freq)*gate;
