#include <SPI.h>
#include <WiFi.h>

char* ssid = "U of Iowa Hawkeyes"; // the name of your network
char* password = "zdilarlinksys";  // your network password
int status = WL_IDLE_STATUS; //network status

unsigned int localPort = 2390;      // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

const short tempPin=0, peltierPin=3; //analog pin A0 for lm35dz & //The N-Channel MOSFET is on digital pin 3
short temperature=0, prevTemp=0, peltierPower=0;

//int peltier_level = map(peltierPower, 0, 99, 0, 255); //do I need it?
//This is a value from 0 to 255 that actually controls the MOSFET
void setup() 
{
  pinMode(tempPin, INPUT);
  pinMode(peltierPin, OUTPUT);
  Serial.begin(9600); //for testing
  status = WiFi.begin(ssid, password);
  if ( status != WL_CONNECTED) { Serial.println("Arduino couldn't get a wifi connection");} 
  else {Serial.print("Arduino is connected to the network");}
}

void loop() 
{
  temperature=readTempSensor(); //read the air temp data from the lm35dz temp sensor
  if(temperature!=prevTemp) //send air temp data to smart phone
  {
    Serial.println(temperature); //change this to a wifi command
    prevTemp=temperature;
  }
  /*test print statments*/
  Serial.print(temperature); 
  Serial.println("* C");
}
/**
 * This function reads in the analog input and converts this input
 * to a temperature (in degrees celsius) by using this ADC formula:
 * temperature = (reference voltage/resolution) * (analog voltage measured) * 100
 * temperature = ((analog voltage measured) * 100 * 5.00 V)/1024)
 * temperature = (analog voltage measured) * 0.48828125
 */
 //LM35DZ
short readTempSensor()
{
  short temperature = analogRead(tempPin); //casting input to integer
  temperature = temperature * 0.48828125;
  return temperature;
}
/**
 * Displaying the wifi status for testing purposes
 */
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
