/*
 * Code for RTC WakeUp from DeepSleep
 */

void initRTC()
{
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(hours + wakeupHours, minutes + wakeupMinutes, seconds + wakeupSeconds);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  rtc.attachInterrupt(alarmMatch);

  if (DEBUG) Serial.println("RTC WakeUp initialized.");
}

void alarmMatch() 
{
  if (DEBUG) Serial.println("WakeUp from DeepSleep.");
  matched = true;
}
