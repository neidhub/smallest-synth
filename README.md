# smallest-synth
i want to build the smallest synthesizer

vision 

* fits in a tiny mints box or something similar
* runs on teensy 4.0
* voices written in faust
  * separate module for master fx?
* 8 voices or more
* no i/o except midi in and audio out
  * power via usb
  * midi in via usb
  * audio out via usb
  * midi in via mini jack (for size)
  * audio out via mini jack (using teensy audio board?)
* completely controlled via midi
  * supporting presets via sysex and stuff (no audio how any of this works)
  * maybe osc is better? who knows

https://github.com/neidhub/smallest-synth/tree/main/generic contains a generic implementation where the voice is just a sine wave and the character of the synth is not influenced by arudino code. this can be used as a template to build other synths off of and will be updated as i learn more about how all of this works. i'm hoping it will be useful because i was looking for something like it and did not find it.

resources

https://github.com/makingsoundmachines/Faust-on-Teensy is where I stole most of the arduino code for handling midi and voice allocation from. they are also very nice people.
