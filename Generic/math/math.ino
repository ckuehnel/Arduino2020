// File: math.ino

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  
  Serial.print("abs(-5) = "); Serial.println(abs(-5));
  Serial.print("abs(-32767) = "); Serial.println(abs(-32767));

  Serial.print("\nconstrain(5, 10, 20) = "); Serial.println(constrain(5, 10, 20));
  Serial.print("constrain(15, 10, 20) = "); Serial.println(constrain(15, 10, 20));
  Serial.print("constrain(25, 10, 20) = "); Serial.println(constrain(25, 10, 20));

  Serial.print("\nconstrain(5., 10., 20.) = "); Serial.println(constrain(5., 10., 20.));
  Serial.print("constrain(15., 10., 20.) = "); Serial.println(constrain(15., 10., 20.));
  Serial.print("constrain(25., 10., 20.) = "); Serial.println(constrain(25., 10., 20.));

  Serial.print("\nmap(512, 0, 1023, 0, 255) = "); Serial.println(map(512, 0, 1023, 0, 255));
  Serial.print("map(128, 0, 255, 0, 1023) = "); Serial.println(map(128, 0, 255, 0, 1023));
  Serial.print("map(0, -10, 10, 0, 1000) = "); Serial.println(map(0, -10, 10, 0, 1000));
  Serial.print("map(-20, -10, 10, 0, 1000) = "); Serial.println(map(-20, -10, 10, 0, 1000));
  Serial.print("map(20, -10, 10, 0, 1000) = "); Serial.println(map(20, -10, 10, 0, 1000));

  Serial.print("\nmax(5, 4) = "); Serial.println(max(5, 4));
  Serial.print("max(5.5, 4.4) = "); Serial.println(max(5.5, 4.4));

  Serial.print("\nmin(5, 4) = "); Serial.println(min(5, 4));
  Serial.print("min(5.5, 4.4) = "); Serial.println(min(5.5, 4.4));

  Serial.print("\npow(2, 10) = "); Serial.println(pow(2, 10));
  Serial.print("pow(2.2, 3.3) = "); Serial.println(pow(2.2, 3.3), 6);

  Serial.print("\nsq(2) = "); Serial.println(sq(2));
  Serial.print("sq(2.5) = "); Serial.println(sq(2.5));
  
  Serial.print("\nsqrt(2) = "); Serial.println(sqrt(2), 6);
  Serial.print("sqrt(2.) = "); Serial.println(sqrt(2.), 6);

  Serial.print("\ncos(PI) = "); Serial.println(cos(PI));
  Serial.print("sin(PI/2) = "); Serial.println(sin(PI/2));
  Serial.print("tan(PI) = "); Serial.println(tan(PI));

  Serial.print("\nPI * RAD_TO_DEG = "); Serial.println(PI * RAD_TO_DEG);
  Serial.print("180° * DEG_TO_RAD = "); Serial.println(180 * DEG_TO_RAD, 6);
}

void loop() {}
