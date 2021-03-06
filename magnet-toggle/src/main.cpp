// Load in our dependencies
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Reference to framework constants
// https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/io.h#L385-L386
// ATtiny25/45/85 vars: https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h
// ATtiny85 specific vars: https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotn85.h

// Set up our constants
#define LED_PIN PB0 // Same as built-in programmer board for ease of use
#define INT0_PIN PB2 // Would use INT0 but that's "6" (bit in GIMSK (interrupt register)) when we need "2" for pin
  // https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h#L346

// Set up our global variables
bool ledState = LOW;

// Define our functions
void setup() {
  // Set all pin modes, with default of INPUT_PULLUP to preserve power (p57, 10.2.6)
  static_assert(PB0 == LED_PIN, "Expected LED_PIN to be PB0");
  pinMode(PB0, OUTPUT);
  pinMode(PB1, INPUT_PULLUP);
  static_assert(PB2 == INT0_PIN, "Expected INT0_PIN to be PB2");
  // DEV: Interrupt pin is now set to trigger when shorted to GND, otherwise would need pull-down resistor with VCC
  //   See Fritzing schematic for clarification
  pinMode(PB2, INPUT_PULLUP);
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PB4, INPUT_PULLUP);
  pinMode(PB5, INPUT_PULLUP);

  // Configure interrupts (p48+)
  GIMSK |= (1 << INT0); // Enable INT0 interrupt
  // In power-down mode, no edges, only level interrupt for INT0 (p34) so no MCUCR config
  // DEV: Technically in this mode, INT0 seems identical to PCINT*

  // Configure sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Minimize power consumption (p36, 7.4)
  //   Disable ADC (set ADEN to 0) (p136, 17.13.2)
  ADCSRA &= ~(1 << ADEN);
  //   Disable analog comparator (set ACD to 1) (p120, 16.2.2)
  ACSR |= (1 << ACD);
  //   Disabled BOD via fuses (see `platformio.ini`) (p41, 8.2.3)
  //   Disabled internal voltage reference, handled by disabling ADC, analog comparator, and BOD (p42, 8.3.1)
  //   Disabled watchdog timer via fuses (see `platformio.ini`) (p43, 8.4)
  //     Can be enabled programmatically even with this fuse set but we don't
  //   Explicitly configured port pins, handled at top of `setup` (p57, 10.2.6)

  // Set up our default LED state
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  // Sleep immediately upon loop, all toggling occurs within `ISR`
  sleep_enable();
  sleep_cpu();

  // Token `delay` to give time to output any logging from `ISR`
  delay(100);
}

// When we receive an external interrupt on INT0
//   https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/interrupt.h#L134-L142
//   https://github.com/vancegroup-mirrors/avr-libc/blob/06cc6ff5e6120b36f1b246871728addee58d3f87/avr-libc/include/avr/iotnx5.h#L365
//   These come from table on p48 of datasheet
ISR(INT0_vect) {
  // If our interrupt occurred on a rising edge (this runs on both rising and falling), then toggle our LED
  if (digitalRead(INT0_PIN) == HIGH) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
