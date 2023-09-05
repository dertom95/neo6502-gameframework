# neo6502-gameframework

highly experimental! first fiddling around with the rp2040-side. 


Palette:
https://lospec.com/palette-list/duel

PicoProbe:

- Got it (kind of) working with rpi pico (picoprobe.uf2) => neo6502
- launch.json with ocd using picoprobe.cfg didn't work. 
- additionally had to add following line to rp2040.cfg ():
```
adapter speed 5000
```
- breakpoints worked at the beginning (again kind of):
  - jumped to main, you could step over and such but once in loop it was over
  - still breaking on crash which is actually very useful
- works quite good for pushing the application to the device
