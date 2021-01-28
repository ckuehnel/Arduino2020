/*
 * Controlling the buzzer
 */
 
#define BUZZER_CHANNEL 0

void melody()
{
  tone(BEEPER, NOTE_C5, 200, BUZZER_CHANNEL);
  noTone(BEEPER, BUZZER_CHANNEL);
  tone(BEEPER, NOTE_E5, 200, BUZZER_CHANNEL);
  noTone(BEEPER, BUZZER_CHANNEL);
  tone(BEEPER, NOTE_G5, 200, BUZZER_CHANNEL);
  noTone(BEEPER, BUZZER_CHANNEL);
  tone(BEEPER, NOTE_C6, 500, BUZZER_CHANNEL);
  noTone(BEEPER, BUZZER_CHANNEL);  
}


void beeps()
{
  for (int i = 0; i <3; i++)
  {
    tone(BEEPER, NOTE_F6, 200, BUZZER_CHANNEL);
    noTone(BEEPER, BUZZER_CHANNEL);
    delay(200);
  }  
}
