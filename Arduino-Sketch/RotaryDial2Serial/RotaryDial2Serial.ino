#include "SoftwareSerial.h"

// define pin numbers
#define LED   0
#define HOOK  1
#define DIAL  2
#define RX    3 
#define TX    4 

SoftwareSerial mySerial(RX, TX);

int count = 0;  // counter for the dial clicks

unsigned long lastclick;  // millis() value for last dial click
unsigned long lasthook;  // millis() value for last hook change
bool hookpos;  // current hook position
bool clicker;  // current clicker position

void setup() {
  // initialize pins
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(DIAL, INPUT);
  pinMode(HOOK, INPUT);
  pinMode(LED, OUTPUT);

  // initialize software serial
  mySerial.begin(115200);  
}

void loop() {
  // when the rotary dial sends a click, register it, count it and set the timer for debouncing
  if (digitalRead(DIAL) == 1 && clicker == 0) {
    clicker = 1;
    lastclick = millis();
    count++;
  }

  // reset the clicker after the debounce timer is up
  if (digitalRead(DIAL) == 0 && lastclick + 80 < millis()) clicker = 0;

  // when there were clicks counted and the last click was more than 200ms ago
  if (count > 0 && lastclick + 200 < millis()) {
    // check if the hook is up. If so, we take the remainder of a division by 10
    // to turn 10 into 0 and send it to the serial connection
    if (hookpos == 0) mySerial.print(count % 10);
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
