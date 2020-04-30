#include "Arduino.h"
#include <TimeLib.h>

#define pLED GPIO2            // LED signalizes operational mode
#define timetosleep 5000      // wake time
#define timetowake 10000      // sleep time
static TimerEvent_t sleep;
static TimerEvent_t wakeUp;
uint8_t lowpower=1;

void onSleep()
{
  Serial.printf("into lowpower mode, %d ms later wake up.\r\n",timetowake);
  digitalWrite(pLED, LOW);
  lowpower=1;
  //timetosleep ms later wake up;
  TimerSetValue( &wakeUp, timetowake );
  TimerStart( &wakeUp );
}
void onWakeUp()
{
  Serial.printf("wake up, %d ms later into lowpower mode.\r\n",timetosleep);
  digitalWrite(pLED, HIGH);
  lowpower=0;
  //timetosleep ms later into lowpower mode;
  TimerSetValue( &sleep, timetosleep );
  TimerStart( &sleep );
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  
  Serial.printf("\nTest Deep Sleep and Timer Wakeup for CubeCell Module...\n");
  Serial.printf("There are %d ms wake time followed by %d ms deep sleep time.\n\n", timetosleep, timetowake);
  boardInitMcu();
  Radio.Sleep( );
  pinMode(pLED, OUTPUT); 
  digitalWrite(pLED, HIGH);
  TimerInit( &sleep, onSleep );
  TimerInit( &wakeUp, onWakeUp );
  onSleep();
}

void loop() {
  if(lowpower){
    //note that lowPowerHandler() run six times the mcu into lowpower mode;
    lowPowerHandler();
  }
  // put your main code here, to run repeatedly:
}
