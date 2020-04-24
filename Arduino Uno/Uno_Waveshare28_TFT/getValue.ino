/*
 * Simulation of measuring values for temperature and humidity
 * Replace this by real access to the choosen sensor
 */
  
float getTemperature()
{
  long temp = random(-400, 700);
  return (float) temp/10.;
}

float getHumidity()
{
  long humi = random(0, 100);
  return (float) humi;
}
