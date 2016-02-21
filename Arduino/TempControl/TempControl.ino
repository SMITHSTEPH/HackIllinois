/*wifi*/
#include <SPI.h>
#include <WiFi.h>
char* ssid = "U of Iowa Hawkeyes"; // the name of your network
char* password = "zdilarlinksys";  // your network password
int status = WL_IDLE_STATUS; //network status
WiFiClient client;
/*These values will be subject to change*/
//IPAddress serverAddr(192,168,1,102); //stephanie's ip address
IPAddress serverAddr(192,168,1,105);  // Google
unsigned int serverPort = 80; //port that server is listening on
const short MAX_BUF=50;
char messageBuffer[MAX_BUF]; //largest message should be 50 bytes
/*pin config*/
const short airTempPin=0, peltierPin=3, peltierTempPin=1; //analog pin A0 for lm35dz & //The N-Channel MOSFET is on digital pin 3
short airTemperature=0, airPrevTemp=0, peltierTemperature=0, prevPeltierTemperature=0, peltierPower=0;

//int peltier_level = map(peltierPower, 0, 99, 0, 255); //do I need it?
//This is a value from 0 to 255 that actually controls the MOSFET
void setup() 
{
  pinMode(airTempPin, INPUT);
  pinMode(peltierPin, OUTPUT);
  Serial.begin(9600); //for testing
  /*wifi*/
  
  status = WiFi.begin(ssid, password);
  if ( status != WL_CONNECTED) { Serial.println("Arduino couldn't get a wifi connection");} //connect to server
  else {Serial.println("Arduino is connected to the network");}
  int response = client.connect(serverAddr,serverPort);
  Serial.println(response);
  //else{Serial.println("Arduino could not connect to server");}
  printWifiStatus();
}

void loop() 
{
  //sendDataToServer("air-" + String(10));
  /*airTemperature=readTempSensor(); //read the air temp data from the lm35dz temp sensor
  if(airTemperature!=airPrevTemp) //send air temp data to smart phone
  {
    sendDataToServer("air-"+String(airTemperature));
    airPrevTemp=airTemperature;
  }
  readDataFromServer();*/
  //readDataFromServer();
  /*test print statments*/
  //Serial.print(temperature); 
  //Serial.println("* C");
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
  short temperature = analogRead(airTempPin); //casting input to integer
  temperature = temperature * 0.48828125;
  return temperature;
}
/**
 * Send data to the server
 */
bool sendDataToServer(String data)
{
  client.println(data);
  return true;
}
/**
 * Reads data from the server
 */
bool readDataFromServer()
{
  short i=0;
  while (client.available() && i < MAX_BUF )
  {
    char c = client.read();
    messageBuffer[i]=c;
    i++;
  }
  //if the server's disconnected, stop the client:
  if (!client.connected())
  {
    disconnectFromServer();
    return false;
  }
  Serial.print("Arduino received from server: ");
  Serial.println(messageBuffer);
  return true;
}
/**
 * disconnect from server
 */
bool disconnectFromServer()
{
   Serial.println();
   Serial.println("Arduino is disconnecting from server.");
   client.stop();
   return true;
}
/**
 * Displaying the wifi status for testing purposes
 */
void printWifiStatus() 
{
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
