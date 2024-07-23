Use Arduino IDE to compile and install this sketch

You need the [ATtinyCore](https://github.com/SpenceKonde/ATTinyCore) installed and use the following settings:

- Board: "ATtiny25/45/85 (No bootloader)"
- Chip: "ATtiny 85" or "ATtiny 45" depending on what you use
- Clock Source (Only set on bootload): "8 MHz (internal)"
- millis ()/micros(): "Enabled"

Connect the board using a USBasp or other compatible ICSP programmer, then burn the bootloader and after that upload the sketch using the programmer (Sketch -> Upload Using Programmer) 

Keep in mind that the phone parts must not be connected while doing this or the upload will fail. 
