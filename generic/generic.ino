// AudioShield libraries and code from Faust
#include <Audio.h>
#include "faustSynth.h"

//AudioOutputI2S out;
//AudioControlSGTL5000 audioShield;

// Create 8 instances of the faustSynth
faustSynth faustSynth0;
faustSynth faustSynth1;
faustSynth faustSynth2;
faustSynth faustSynth3;
faustSynth faustSynth4;
faustSynth faustSynth5;
faustSynth faustSynth6;
faustSynth faustSynth7;


// Teensy Audio Library stuff - Routing the individual instances to the dac
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code


AudioMixer4              mixer1;         //xy=517.2000122070312,200.1999969482422
AudioMixer4              mixer2; //xy=543.2000122070312,439.20001220703125
AudioMixer4              mixer3; //xy=712.2000122070312,281.20001220703125
AudioConnection          patchCord1(faustSynth0, 0, mixer1, 0);
AudioConnection          patchCord3(faustSynth1, 0, mixer1, 1);
AudioConnection          patchCord2(faustSynth2, 0, mixer1, 2);
AudioConnection          patchCord7(faustSynth3, 0, mixer1, 3);
AudioConnection          patchCord4(faustSynth4, 0, mixer2, 0);
AudioConnection          patchCord6(faustSynth5, 0, mixer2, 1);
AudioConnection          patchCord5(faustSynth6, 0, mixer2, 2);
AudioConnection          patchCord8(faustSynth7, 0, mixer2, 3);
AudioConnection          patchCord9(mixer1, 0, mixer3, 0);
AudioConnection          patchCord10(mixer2, 0, mixer3, 1);

// audio shield: headphones & line-out
AudioControlSGTL5000 audioShield;
AudioOutputI2S      audioShieldOut;
AudioConnection          patchCord11(mixer3, audioShieldOut);

// onboard dac
//AudioOutputAnalog        dac1;           //xy=929.1999969482422,272.1999969482422
//AudioConnection          patchCord11(mixer3, dac1);

// GUItool: end automatically generated code


// This array stores tempered frequencies for all 127 MIDI Note numbers.
// These values will be used as quantized pitch for the fundamental harmonic.
// Note that the harmonic partials are set in the Faust code and, are integer multipliers of the fundamental frequency.

//     C         C#        D         D#        E         F         F#        G         G#        A         A#        B
const float NoteNumToFreq[] = {
  8.18,     8.66,     9.18,     9.72,    10.30,    10.91,    11.56,    12.25,    12.98,    13.75,    14.57,    15.43,
  16.35,    17.32,    18.35,    19.45,    20.60,    21.83,    23.12,    24.50,    25.96,    27.50,    29.14,    30.87,
  32.70,    34.65,    36.71,    38.89,    41.20,    43.65,    46.25,    49.00,    51.91,    55.00,    58.27,    61.74,
  65.41,    69.30,    73.42,    77.78,    82.41,    87.31,    92.50,    98.00,   103.82,   110.00,   116.54,   123.47,
  130.81,   138.59,   146.83,   155.56,   164.81,   174.61,   184.99,   195.99,   207.65,   220.00,   233.08,   246.94,
  261.63,   277.18,   293.66,   311.13,   329.63,   349.23,   369.99,   391.99,   415.31,   440.00,   466.16,   493.88,
  523.25,   554.37,   587.33,   622.25,   659.26,   698.46,   739.99,   783.99,   830.61,   880.00,   932.32,   987.77,
  1046.50,  1108.73,  1174.66,  1244.51,  1318.51,  1396.91,  1479.98,  1567.98,  1661.22,  1760.00,  1864.66,  1975.53,
  2093.00,  2217.46,  2349.32,  2489.02,  2637.02,  2793.83,  2959.96,  3135.96,  3322.44,  3520.00,  3729.31,  3951.07,
  4186.01,  4434.92,  4698.64,  4978.03,  5274.04,  5587.65,  5919.91,  6271.93,  6644.88,  7040.00,  7458.62,  7902.13,
  8372.02,  8869.84,  9397.27,  9956.06, 10548.08, 11175.30, 11839.82, 12543.85
};

// MIDI polyphony - store the last note, and an array of all current notes.
// Value -1 means the note is off (not sounding).
byte lastNote = 0;
// set number of voices here!!
//hacky hack: uncomment the line matching how many voices you want

//int StoredNotes[1] = {-1};
//int StoredNotes[2] = { -1, -1};
//int StoredNotes[3] = {-1, -1, -1};
//int StoredNotes[4] = { -1, -1, -1, -1};
//int StoredNotes[5] = {-1, -1, -1, -1, -1};
//int StoredNotes[6] = {-1, -1, -1, -1, -1, -1};
//int StoredNotes[7] = {-1, -1, -1, -1, -1, -1, -1};
int StoredNotes[8] = { -1, -1, -1, -1, -1, -1, -1, -1};


void setup() {
  AudioMemory(20);

  // Enable the AudioShield
  AudioMemory(31);
  audioShield.enable();
  audioShield.volume(0.9);

  // set the mixer gains flat
  mixer1.gain(0, 0.25);
  mixer1.gain(1, 0.25);
  mixer1.gain(2, 0.25);
  mixer1.gain(3, 0.25);
  mixer2.gain(0, 0.25);
  mixer2.gain(1, 0.25);
  mixer2.gain(2, 0.25);
  mixer2.gain(3, 0.25);
  mixer3.gain(0, 0.5);
  mixer3.gain(1, 0.5);

  // Handles for the USB MIDI callbacks
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(setCC);

  // just for debug
  Serial.begin(9600);
  Serial.println("setup done");

}


void loop() {

  usbMIDI.read();

}


// // no The integer CC value 0 .. 127 gets converted to float
// transmit midi cc into the faust synth

void setCC(byte channel, byte control, byte value) {
  // make a nice little string for the setParamValue function
  char controlstring[3];
  sprintf(controlstring, "cc%d", control);

  // +1 because code goes crazy when it hits 0 idk why
  float valuefloat = float(value+1);
  
  faustSynth0.setParamValue(controlstring, valuefloat);
  faustSynth1.setParamValue(controlstring, valuefloat);
  faustSynth2.setParamValue(controlstring, valuefloat);
  faustSynth3.setParamValue(controlstring, valuefloat);
  faustSynth4.setParamValue(controlstring, valuefloat);
  faustSynth5.setParamValue(controlstring, valuefloat);
  faustSynth6.setParamValue(controlstring, valuefloat);
  faustSynth7.setParamValue(controlstring, valuefloat);

}




// Callback for incoming NoteOn messages
// Handling the voice allocation here.
void myNoteOn(byte channel, byte note, byte velo) {
  // When using MIDIx4 or MIDIx16, usbMIDI.getCable() can be used
  // to read which of the virtual MIDI cables received this message.
  float velocity = float(velo) / 127;
  size_t i = 0;
  while (StoredNotes[i] != -1 && i < sizeof(StoredNotes)) {
    i++;
  }
  if (i == sizeof(StoredNotes)) {
    i = 0;
  };
  Serial.print("on");
  Serial.println(i);
  StoredNotes[i] = int(note);
  switch (i) {
    case 0:
      faustSynth0.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth0.setParamValue("gain", velocity);
      faustSynth0.setParamValue("gate", 1);
      break;
    case 1:
      faustSynth1.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth1.setParamValue("gain", velocity);
      faustSynth1.setParamValue("gate", 1);
      break;
    case 2:
      faustSynth2.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth2.setParamValue("gain", velocity);
      faustSynth2.setParamValue("gate", 1);
      break;
    case 3:
      faustSynth3.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth3.setParamValue("gain", velocity);
      faustSynth3.setParamValue("gate", 1);
      break;
    case 4:
      faustSynth4.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth4.setParamValue("gain", velocity);
      faustSynth4.setParamValue("gate", 1);
      break;
    case 5:
      faustSynth5.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth5.setParamValue("gain", velocity);
      faustSynth5.setParamValue("gate", 1);
      break;
    case 6:
      faustSynth6.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth6.setParamValue("gain", velocity);
      faustSynth6.setParamValue("gate", 1);
      break;
    case 7:
      faustSynth7.setParamValue("freq", NoteNumToFreq[note]);
      faustSynth7.setParamValue("gain", velocity);
      faustSynth7.setParamValue("gate", 1);
      break;
  }
}

// Callback for incoming NoteOff messages
// Releasing voices to be re-allocated here.

void myNoteOff(byte channel, byte note, byte velocity) {
  size_t i = 0;
  for (i = 0; i < sizeof(StoredNotes); i++) {
    if (StoredNotes[i] == note) {
      StoredNotes[i] = -1;
      break;
    }
  }
  Serial.print("off");
  Serial.println(i);
  switch (i) {
    case 0:
      faustSynth0.setParamValue("gate", 0);
      break;
    case 1:
      faustSynth1.setParamValue("gate", 0);
      break;
    case 2:
      faustSynth2.setParamValue("gate", 0);
      break;
    case 3:
      faustSynth3.setParamValue("gate", 0);
      break;
    case 4:
      faustSynth4.setParamValue("gate", 0);
      break;
    case 5:
      faustSynth5.setParamValue("gate", 0);
      break;
    case 6:
      faustSynth6.setParamValue("gate", 0);
      break;
    case 7:
      faustSynth7.setParamValue("gate", 0);
      break;
  }
}



