/*
 * File: Uno_TCS3200.ino
 * 
 * based on https://wiki.dfrobot.com/TCS3200_Color_Sensor__SKU_SEN0101_
 * Modified by Claus Kuehnel 2020-02-12 info@ckuehnel.ch
 */

const int s0 = 3, s1 = 4, s2 = 5, s3 = 6;
int flag = 0;
int counter = 0;
int countR = 0, countG = 0, countB = 0;
int i = 0;

#define DEBUG 1

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("\nReading TCS3200 Color Sensor...");
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT); 
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
}

void TCS()
{
  digitalWrite(s1,HIGH);  // Frequency scaling 2% (300 Hz)
  digitalWrite(s0,LOW);
  flag = 0;
  attachInterrupt(0, ISR_INTO, CHANGE);
  timer2_init();
}

void ISR_INTO() 
{
  counter++;
}

void timer2_init(void)
{
  TCCR2A = 0x00;
  TCCR2B = 0x07;  //the clock frequency source 1024 points
  TCNT2 =  100;   //10 ms overflow again
  TIMSK2 = 0x01;  //allow interrupt
}

ISR(TIMER2_OVF_vect) //the timer 2, 10ms interrupt overflow again.
{
  TCNT2 = 100;
  flag++;
  if(flag == 1) counter = 0;
  else if(flag == 2)
  {
     digitalWrite(s2,LOW);  // Red filter
     digitalWrite(s3,LOW); 
     countR = counter/1.051;
     if (DEBUG)
     {
       Serial.print("red = ");
       Serial.println(countR,DEC);
     }
     digitalWrite(s2,HIGH); // Green filter
     digitalWrite(s3,HIGH);   
  }
  else if(flag==3)
  {
    countG = counter/1.0157;
    if (DEBUG)
    {
      Serial.print("green = ");
      Serial.println(countG,DEC);
    }
    digitalWrite(s2,LOW); // Blue filter
    digitalWrite(s3,HIGH); 
  }
  else if(flag==4)
  {
    countB = counter/1.114;
    if (DEBUG)
    {
      Serial.print("blue = ");
      Serial.println(countB,DEC);    
    }
    digitalWrite(s2,LOW); // No filter
    digitalWrite(s3,LOW);
  }
  else
  {
    flag = 0; 
    TIMSK2 = 0x00;
  }
  counter = 0;
  delay(2);
}

void loop()
{
  delay(10);
  TCS();
  if((countR > 10)||(countG > 10)||(countB > 10))
  {
    if (DEBUG) Serial.println("Signals over noise");
    if((countR > countG)&&(countR > countB))
    {
      Serial.println("Found red");
      delay(1000);
    }
    else if((countG > countR)&&(countG > countB))
    {
      Serial.println("Found green");
      delay(1000);
    } 
    else if((countB > countG)&&(countB > countR))
    {
      Serial.println("Found blue");
      delay(1000);
    }
  }
  else 
  {
    if (DEBUG) Serial.println("Noisy signals - no color information");
    delay(2000);       
  }
}
