/*
 * File: UnoWiFiR2_TimerInterrupt_Debug.ino
 * 
 * based on Argument_None.ino
 * For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/megaAVR_TimerInterrupt
 * Licensed under MIT license
 * 
 * Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
 * Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
 * The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
 * Therefore, their executions are not blocked by bad-behaving functions / tasks.
 * This important feature is absolutely necessary for mission-critical tasks.
 */

#if !( defined(__AVR_ATmega4809__) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) || \
      defined(ARDUINO_AVR_ATmega4809) || defined(ARDUINO_AVR_ATmega4808) || defined(ARDUINO_AVR_ATmega3209) || \
      defined(ARDUINO_AVR_ATmega3208) || defined(ARDUINO_AVR_ATmega1609) || defined(ARDUINO_AVR_ATmega1608) || \
      defined(ARDUINO_AVR_ATmega809) || defined(ARDUINO_AVR_ATmega808) )
  #error This is designed only for Arduino or MegaCoreX megaAVR board! Please check your Tools->Board setting
#endif

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "megaAVR_TimerInterrupt.h"

#define TIMER1_INTERVAL_MS    5000  // Timer intervall 5 sec

#ifndef LED_BUILTIN
  #define LED_BUILTIN   13
#endif

unsigned int count = 0;

//====MicroDebug Includes==========================================================
// This is optional, but set to false to disable
#define SERIAL_DEBUG true
#include <SerialDebug.h>

// This is optional, but set to false to disable
#define LED_DEBUG true
#include <LedDebug.h>

// Set this to override the default value
#define LED_DEBUG_PIN LED_BUILTIN

// Set the following if you wish to override the default
#define SERIAL_DEBUG_SEPARATOR ", "

int free() 
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
//================================================================================

void TimerHandler1(void)
{
  count = 0; // Reset counter variable
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.print(F("\nStarting UnoWiFiR2_TimerInterrupt.ino on "));
  Serial.println(BOARD_NAME);
  Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.print(F("TCB Clock Frequency = ")); 

#if USING_16MHZ  
  Serial.println(F("16MHz for highest accuracy"));
#elif USING_8MHZ  
  Serial.println(F("8MHz for very high accuracy"));
#else
  Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency
  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    Serial.println(F("Starting  ITimer1 OK")); 
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  SERIAL_DEBUG_SETUP(9600);
  DEBUG(F("Debugging w/ MicroDebug Library by https://github.com/rlogiacco/MicroDebug"));
  DEBUG(F("Available memory"), free());
}

void loop() 
{
  if (count == 0) 
  {
    DEBUG(F("ISR cleared count"));
    PULSE(2);
  }
  count++;
  Serial.print("Count = ");
  Serial.println(count);
  delay(600);
}
