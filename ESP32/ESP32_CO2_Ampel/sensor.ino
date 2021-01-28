/*
 * Initialization of sensors SGP30 & BME280 and query of measuring results
 */
 
void sensorInit()
{
  if (!sgp.begin())
  {
    Serial.println("SGP30 sensor not found :(");
    //while(1);
  }
  else
  {
    Serial.print("Found SGP30 serial #");
    Serial.print(sgp.serialnumber[0], HEX);
    Serial.print(sgp.serialnumber[1], HEX);
    Serial.println(sgp.serialnumber[2], HEX);
  }
  if (!bmx280.begin()) 
  {
    Serial.println("BMx280 sensor not found :(");
    while(1);
  }
  if (bmx280.isBME280())
    Serial.println("BMx280 Sensor is a BME280");
  else
    Serial.println("BMx280 Sensor is a BMP280");

  //reset sensor to default parameters.
  bmx280.resetToDefaults();

  //by default sensing is disabled and must be enabled by setting a non-zero
  //oversampling setting.
  //set an oversampling setting for pressure and temperature measurements. 
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  //if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280())
    bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
  Serial.println("Sensor Initialization finshed.");
}

void getSGP30data()
{
  if (!sgp.IAQmeasure()) 
  {
    Serial.println("Measurement failed");
    return;
  }
  TVOC = sgp.TVOC;
  eCO2 = sgp.eCO2;
  String sTVOC = "TVOC " + String(TVOC) + " ppb\t"; Serial.print(sTVOC);
  String seCO2 = "eCO2 " + String(eCO2) + " ppm";   Serial.println(seCO2);
}

void getBME280data()
{
  //start a measurement
  if (!bmx280.measure())
  {
    Serial.println("could not start measurement, is a measurement already running?");
    return;
  }
  //wait for the measurement to finish
  do
  {
    delay(100);
  } while (!bmx280.hasValue());

  temp  = bmx280.getTemperature();
  String sTemp = "\nTemp " + String(temp) + " *C\t"; Serial.print(sTemp);
  
  if (bmx280.isBME280()) 
  {
    humi = bmx280.getHumidity();
    String sHumi = "Humi " + String(humi) + " %rH"; Serial.println(sHumi);
  }
  else Serial.println();

  press = bmx280.getPressure();  
  String sPress = "Press " + String(press) + " Pa"; Serial.println(sPress);
}

void getEnvData()
{
  getBME280data();
  getSGP30data();
}
