#include "RunningAverage.h"

RunningAverage runAvg(30);

int peltier = 3; //The N-Channel MOSFET is on digital pin 3
int power = 0; //Power level fro 0 to 99%
int peltier_level = map(power, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET
const short tempPin=0;
short temperature=0, prevTemp=0;

char ssid[] = "U of Iowa Hawkeyes";
char pass[] = "zdilarlinksys";
//int status = WL_IDLE_STATUS;
WiFiServer server(23);

void setup(){
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
  //pinMode(peltier, OUTPUT);
}

void loop(){
  temperature=readTempSensor(); //read the air temp data from the lm35dz temp sensor
  runAvg.addValue(temperature);

  
  /*if(temperature!=prevTemp) //send air temp data to smart phone
  {
    //Serial.println(temperature); //change this to a wifi command
    prevTemp=temperature;
  }*/
  /*test print statments*/
  //Serial.print(temperature); 
  //Serial.println("* C");
  char option;

  if(Serial.available() > 0)
  {
    option = Serial.read();
    if(option == 'a') 
      power += 5;
    else if(option == 'z')
      power -= 5;
  
    if(power > 99) power = 99;  
    if(power < 0) power = 0;
  
    peltier_level = map(power, 0, 99, 0, 255);
  }

  Serial.print("Temperature=");
  //Serial.print(runAvg.getAverage());
  Serial.print(temperature);
  Serial.print(" Power=");
  Serial.print(power);
  Serial.print(" PLevel=");
  Serial.println(peltier_level);
  
  analogWrite(peltier, peltier_level); //Write this new value out to the port

}gy

short readTempSensor()
{
  float temperature = analogRead(tempPin); //casting input to integer
  temperature = temperature * 4.8828125;
  temperature = (temperature - 500)/10.0;
  temperature = temperature*1.8 + 32;
  
  // Calibration
  temperature -= 8;
  return ((short) temperature);b 
}
