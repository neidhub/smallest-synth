import("stdfaust.lib");
declare options "[midi:on]";

freq = nentry("freq", 400, 0, 20000, 0.1);
gain = nentry("gain", 0.5, 0, 1, 0.01);
gate = en.adsr(0,0.1,0.8,0.4, button("gate")) ;

//yes we have to do this every time
//faust UI mapping the midi 0..127 to the range of slider doesnt work for us
//so we do it manually
//                                                       make it log (optional)
cutoff = hslider("cc1[midi:ctrl 1]", 127,0,127,1) /255 : ba.lin2LogGain /2 : * (maxout-minout) + minout : si.smoo
with{
    minout = 1;
    maxout = 22000;
};



// Teensy Oscillator
// lowering the resolution of the sine oscillators' lookup table 
// in order to make computation easier on the Teensy MCU and memory
// from https://github.com/makingsoundmachines/Faust-on-Teensy/blob/main/AdditiveSynth_Faust/faustAdditive.dsp

// oscTeensy(f) = rdtable(tablesize, os.sinwaveform(tablesize), int(os.phasor(tablesize,f)))
// with{
//     tablesize = 1 << 13; // instead of 1 << 16
// };


//process = os.triangle(freq)*gate : fi.lowpass(1, cutoff);
process = os.triangle(freq)*gate*gain : fi.resonlp(cutoff,1,0.7);
//process = cutoff;