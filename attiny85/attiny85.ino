// Load in our dependencies
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Set up our constants
int ledPin = PB0; // Same as built-in programmer board for ease of use
int interruptPin = PCINT2; // PB2
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  // DEV: Unsure if it's a good idea to reuse `PCINT2` for both `pinMode` and `PCMSK` but it seems to work
  // DEV: We tried/failed to use `INPUT_PULLUP` so this must be used with an external pull-up resistor
  pinMode(interruptPin, INPUT);

  // Configure interrupt/sleep
  PCMSK |= (1 << interruptPin); // Enable interrupt handler (ISR) for our chosen interrupt pin
  GIMSK |= (1 << PCIE); // Enable PCINT interrupt in the general interrupt mask
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

// When we receive interrupt 0
ISR(PCINT0_vect) {
  // If our interrupt occurred on a rising edge (this runs on both rising and falling), then toggle our LED
  if (digitalRead(interruptPin) == HIGH) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
