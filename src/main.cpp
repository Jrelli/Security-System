#include <Arduino.h>
#include "hardware.h"

typedef enum{
  LOCKDOWN    = 0x00,
  UNLOCKED    = 0x01
} DeviceMode;

unsigned long currentTimeInMS = 0;
unsigned long lastScreenUpdate = 0;
const long fiveSecInMS = 5000;
DeviceMode mode = LOCKDOWN;

// put your setup code here, to run once:
void setup(){
  Serial.begin(115200);
  
  setupScreen();

  pinMode(PIN_REED_SWITCH_NC, INPUT_PULLUP); // set the reed switch's pin's mode


}

// put your main code here, to run repeatedly:
void loop(){
  currentTimeInMS = millis();

  // update screen every 5 seconds
  if(currentTimeInMS - lastScreenUpdate > fiveSecInMS){
    updateScreen();
    lastScreenUpdate = millis();
  }

  switch (mode){
    case LOCKDOWN:
      /* code */
      break;
    case UNLOCKED:
      break;
    default:
      break;
  }
}