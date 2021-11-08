// Method to print the reason by which ESP32 has been awaken from sleep

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :     Serial.println("\nWakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 :     Serial.println("\nWakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER :    Serial.println("\nWakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("\nWakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP :      Serial.println("\nWakeup caused by ULP program"); break;
    default :                        Serial.printf("\nWakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
