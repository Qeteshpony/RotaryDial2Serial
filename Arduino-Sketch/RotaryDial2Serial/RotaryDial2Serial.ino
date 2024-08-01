#include "SoftwareSerial.h"

// define pin numbers
#define LED   0
#define HOOK  1
#define DIAL  2
#define RX    3 
#define TX    4 

SoftwareSerial mySerial(RX, TX);

int count = 0;  // counter for the dial clicks

long int lastclick;  // millis() value for last dial click
long int lasthook;  // millis() value for last hook change
bool hookpos;  // current hook position

void dial() {
  // gets invoked by interrupt every time the dial clicks
  // check if the last click is more than 10 ms ago for debouncing the switch, if so: count it
  if (lastclick + 10 < millis()) count++;
  lastclick = millis();
}

void setup() {
  // initialize pins
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(DIAL, INPUT);
  pinMode(HOOK, INPUT);
  pinMode(LED, OUTPUT);

  // attach int0 (PB2) and use dial() as the callback on a falling edge
  attachInterrupt(0, dial, FALLING);

  // initialize software serial
  mySerial.begin(115200);  
}

void loop() {
  // when there were clicks counted and the last click was more than 200ms ago
  if (count > 0 && lastclick + 200 < millis()) {
    // check if the hook is up. If so divide the clicks by two to get the dialed number 
    // and send it to the serial connection
    // we round the number because sometimes a click doesnt get through on old dials
    // after that we take the remainder of a division by 10 to turn 10 into 0
    if (hookpos == 0) mySerial.print(int(float(count) / 2 + 0.5) % 10);
    // reset the click counter
    count = 0;
  }

  // when the hook changed and the last change was more than 50 ms ago
  bool newhookpos = digitalRead(HOOK);
  if (lasthook + 50 < millis() && newhookpos != hookpos) {
    // store time of this hookchange for debouncing
    lasthook = millis();
    // update the hookpos
    hookpos = newhookpos;
    // send new hookpos to serial
    if (hookpos == 0) mySerial.print("u");
    else mySerial.print("d");
  }

  // switch the LED off for 25ms if a click was received, turn it only on if the hook is up
  if (lastclick + 25 > millis()) digitalWrite(LED, 0);
  else digitalWrite(LED, 1 - hookpos);
}
