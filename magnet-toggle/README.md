# lantern-wall/magnet-toggle
Magnet toggle source code for a lantern wall lantern

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

## Architecture
We use PlatformIO for our project, to avoid dancing between different Arduino IDE setups

As well as document our setup itself as code (e.g. clock speed, fuses)

## Notes
- Starting point for current code: 0.2uA LED off, 1.67mA LED on
- We tried downclocking it but have bricked it .\_. https://www.avrfreaks.net/forum/attiny85-configured-128-khz-internal-oscillator-does-not-program-anymore
