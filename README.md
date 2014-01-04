Dekrispator
===========

Experimental synthesizer based on an STM32F4 Discovery board with USB Host function for interaction and automatic mode.
Flash your board with "dekrispator.hex" and enjoy hours and hours of sweet music coming of it ! Sounds can be loud so beware !

When powering up the board, the synth starts in "demo mode" unless you press and maintain a little the user button before power. In that case the synth is in "user mode".

Demo mode : The synth works on its own, sounds are perpetually changing. If you press the user button, sounds and effects stop always changing and the red LED lights on : the synth parameters are frozen. Press once again the user button and the red LED lights off : unfreeze.

User mode : Connect an USB MIDI controller (like Korg NanoKontrol...) to the board's micro USB connector and control the synth.
The MIDI mapping of the synth controls are in MIDI_mapping.ods file. I nearly used the factory settings of Korg NanoKontrol V1 (some momentary/toggle buttons modified).
The MIDI USB Host driver is rudimentary and has still some defects !

The orange LED reflects the CPU usage : when it's dim CPU gets overloaded.

Ready to use files are in Ressources folder.

Dekrispator features :
  monophonic
  
