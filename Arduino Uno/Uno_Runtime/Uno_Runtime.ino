/*
 * File: Uno_Runtime.ino
 * 
 * Measure runtime of code sequences
 * Claus Kühnel 2020-02-17 info@ckuehnel.ch
 * 
 */
 void setup() 
 {
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  Serial.println("\nRuntime measurment of code sequence in program ");
  Serial.println(__FILE__);

  unsigned long starttime = micros();
  // put your code between the comment lines
  /***************************************************/
  int x = analogRead(0);
  int y = analogRead(1);
  int s = analogRead(2);
  /***************************************************/
  unsigned long endtime = micros();
  Serial.print("Runtime of code sequence = ");
  Serial.print(endtime - starttime);
  Serial.println(" us");
}

void loop() {}
