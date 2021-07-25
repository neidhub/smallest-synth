this folder contains a generic poly synth written in faust along with an arduino file which handles midi and audio i/o.
faust polyphony options are not used as they are currently not operational for teensy (?) (https://github.com/grame-cncm/faust/issues/507).
the faust .dsp file needs to be compiled into a module for teensy audio library with
```
faust2teensy -lib faustSynth.dsp
```
it is then included in the arduino file. I have included the compiled .cpp and .h file for convenience.
I recommend using teensyduino (https://www.pjrc.com/teensy/teensyduino.html) to compile and flash to teensy. USB type midi must be set in teensyduino.
setup by default uses the teensy audio shield (https://www.pjrc.com/store/teensy3_audio.html) but the on board dac can be used too. this configuration is commented in the .ino file.
per default 8 voices are configured, less are supported, configuration is also included in the .ino file as comments. you can add more by extending the switch statements (are they even needed? i just copied them from an example).
this code will run on teensy 3.2 but it will crash badly using 2 or more voices at the same time. teensy 4.0 works fine

