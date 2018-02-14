/*
 MIDI note player

 This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
 If this circuit is connected to a MIDI synth, it will play
 the notes F#-0 (0x1E) to F#-5 (0x5A) in sequence.


 The circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

 created 13 Jun 2006
 modified 13 Aug 2012
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Midi

 */

const int buttonPin = 7;
const int bendPin = A0;
int oldButtonReading = HIGH;
int oldBendReading;

void setup() {
  //  Set MIDI baud rate:
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(31520);
  oldBendReading = analogRead(bendPin);
}

void loop() {
  int buttonReading = digitalRead(buttonPin);

  int note = 0x5A;

  if (buttonReading == LOW && buttonReading != oldButtonReading) {
    noteOn(0x90, note, 0x45);
  }
  oldButtonReading = buttonReading;


  int threshold = 0;
  int bendReading = analogRead(bendPin);

  // make sure it is over threshold and changes from prev reading
  if (bendReading > threshold && abs(bendReading - oldBendReading) > 0) {
    // midi high byte
    byte msb = highByte(bendReading << 1);

    // midi low byte
    byte lsb = lowByte(bendReading);
    bitWrite(lsb, 7, 0);

    noteOn(0xE0, lsb, msb);
  }
  oldBendReading = bendReading;

  //Serial.print("bendReading: ");
  //Serial.println(bendReading);
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

