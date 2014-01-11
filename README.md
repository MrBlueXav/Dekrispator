Dekrispator
===========

Transform your STM32F4 Discovery board into an experimental music synthesizer or autonomous noise box with USB MIDI Host function for interaction !

- - - -

**Usage**

Flash your board with "dekrispator.hex" (in Ressources folder) and enjoy hours and hours of sweet music coming of it ! Sounds can be loud so beware !

When powering up the board, the synth starts in "demo mode" unless you press and maintain a little the user button before power. In that case the synth enters "user mode".

*Demo mode* : The synth works on its own, sounds are perpetually changing. If you press the user button, sounds and effects stop always changing and the red LED lights on : the synth parameters are frozen. Press once again the user button and the red LED lights off : unfreeze.

*User mode* : Connect an USB MIDI controller (like Korg NanoKontrol...) to the board's micro USB connector and control the synth.
The MIDI mapping of the synth controls are in MIDI_mapping.ods file. I nearly used the factory settings of Korg NanoKontrol V1 (some momentary/toggle buttons modified).

The MIDI USB Host driver is rudimentary and has still some issues !

The orange LED reflects the CPU usage : when it's dim CPU gets overloaded.

Ready to use files are in Ressources folder.

- - - -

**Dekrispator features** :

 * monophonic
 * all digital !
 * oscillators with :
  * low aliased analog waveforms, 
  * 4 OP FM generator
  * multisaw (several saws with individual "drifters")
  * 10 sine additive generator
  * noise
 * vibrato
 * tremolo
 * drive/distortion
 * 16 step random sequencer
 * several scales to choose from
 * 2 parallel filters LP/BP/HP with LFOs
 * Effects :
  * Echo
  * Chorus/Flanger
 * random sound/FX patch generator


  
