// Based on
//   https://learn.sparkfun.com/tutorials/h2ohno#low-power-attiny
//   https://www.instructables.com/id/ATtiny85-Interrupt-Barebones-Example/
//   http://www.technoblogy.com/show?KX0
//   https://forum.arduino.cc/index.php?topic=524949.msg3580423#msg3580423
// Load in our dependencies
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Reference to constants
// https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/io.h#L385-L386
// ATtiny25/45/85 vars: https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h
// ATtiny85 specific vars: https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotn85.h

// Set up our constants
int ledPin = PB0; // Same as built-in programmer board for ease of use
int interruptPin = PB2; // INT0 pin, but INT0 is "6" (bit in GIMSK (interrupt register)) when we need "2" for pin
  // https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h#L346
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  // DEV: Interrupt pin is now set to trigger when shorted to GND, otherwise would need pull-down resistor with VCC
  //   See Fritzing schematic for clarification
  pinMode(interruptPin, INPUT_PULLUP);

  // Configure interrupt/sleep
  PCMSK |= (1 << interruptPin); // Enable interrupt handler (ISR) for our chosen interrupt pin
  GIMSK |= (1 << INT0); // Enable PCINT interrupt in the general interrupt mask
  // GIMSK |= (1 << PCIE); // Enable PCINT interrupt in the general interrupt mask
  ADCSRA &= ~(1<<ADEN); // Disable ADC, saves ~230uA
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Set up our default LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // Sleep immediately upon loop, all toggling occurs within `ISR`
  sleep_enable();
  sleep_cpu();

  // Token `delay` to give time to output any logging from `ISR`
  delay(100);
}

// When we receive an external interrupt on INT0
// DEV (pretty confident): PCINT0_vect is pin change interrupt request which is unrelated to INT0 interrupt
//   We cannot use this vector due to it being disabled in power-down, see 7.1 in datasheet, page 34
// TODO: Understand why `INT0_vect` doesn't work...
//   https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h#L365
//   These come from table on p48 of datasheet
ISR(INT0_vect) {
  // If our interrupt occurred on a rising edge (this runs on both rising and falling), then toggle our LED
  if (digitalRead(interruptPin) == HIGH) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
