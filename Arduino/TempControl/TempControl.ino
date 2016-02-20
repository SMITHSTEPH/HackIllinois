
const short tempPin=0; //analog pin A0 for lm35dz
short temperature=0;
void setup() 
{
  pinMode(tempPin, INPUT);
  Serial.begin(9600); //for testing
}

void loop() 
{
  temperature=readTempSensor();
  Serial.print(temperature);
  Serial.println("* C");
}
/**
 * This function reads in the analog input and converts this input
 * to a temperature (in degrees celsius) by using this ADC formula:
 * temperature = (reference voltage/resolution) * (analog voltage measured) * 100
 * temperature = (analog voltage measured) * 100 * (5.00 V/1024)
 * temperature = (analog voltage measured) * 0.48828125
 */
 //LM35DZ
short readTempSensor()
{
  short temperature = analogRead(tempPin); //casting input to integer
  temperature = temperature * 0.48828125;
  return temperature;
}
