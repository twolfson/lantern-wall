# lantern-wall/magnet-toggle
Magnet toggle source code for a lantern wall lantern

TODO: See TODOs

## Getting started
**Requirements:**

- ATtiny85 programmer, e.g. [Tiny AVR Programmer][]
- ATtiny85

[Tiny AVR Programmer]: https://www.sparkfun.com/products/11801

**Steps:**

Connect the ATtiny85 and ATtiny85 programmer to your computer, then run the following in a terminal:

```bash
# If PlatformIO isn't yet installed, we recommend installing it via `virtualenvwrapper`
# mkvirtualenv lantern-wall

# Configure out ATtiny85 fuses
platformio run --target fuses
# Processing attiny85 (platform: atmelavr; board: attiny85; framework: arduino)
# ...
# avrdude done.  Thank you.

# Program our board
platformio run --target upload
# Processing attiny85 (platform: atmelavr; board: attiny85; framework: arduino)
# ...
# avrdude done.  Thank you.
```

The ATtiny85 is now programmed and ready to be used in a circuit.

Please see the [Fritzing][] files (`.fzz`) for wiring layout

[Fritzing]: https://fritzing.org/home/

## Documentation
### Development
If rewriting the script, then we recommend doing all dev on the programmer itself:

- Connect jumper wires to interrupt pin (PB2) and GND, then connect them to simulate a switch closing

If doing more detailed exercises like determining power usage, then V_CC will need to be proxied somehow which cannot be done in place

As a reminder: A 3.3V powered ATtiny cannot be powered by a 5V USB jumper, not only due to being on a different V_CC and GND but also due to different logic levels

Additionally, using 5V power with an LED might cause burnout, if the resistor is too weak

In short, it's probably best to build a development board which has a resistor sturdy enough for 5V and headers for jumping everything from the programmer board (including a V_CC proxy)

## Architecture
We use PlatformIO for our project, to avoid dancing between different Arduino IDE setups

As well as document our setup itself as code (e.g. clock speed, fuses)

## Notes
- Starting point for current code: 0.2uA LED off, 1.67mA LED on
- We tried downclocking it but have bricked the ability to program it .\_. https://www.avrfreaks.net/forum/attiny85-configured-128-khz-internal-oscillator-does-not-program-anymore
  - The ATtiny85 still does work for our lanterns, but yea. Stuck at unable to program =/
  - Going to put it off to the side
