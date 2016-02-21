#include <SPI.h> //libraries for wifi communication
#include <WiFi.h>
#include "RunningAverage.h"//for setting peltier temp
/*wifi*/
char* ssid="U of Iowa Hawkeyes"; //network name SSID 
char* password = "zdilarlinksys";  //network password
int status = WL_IDLE_STATUS;
IPAddress ip(192, 168, 1, 130); //static IP address
WiFiServer server(23); //port that arduino wifi shield is listening on
boolean alreadyConnected = false; //whether or not the client was connected previously
bool isServer=false;
WiFiClient client;
/*peltier*/
const short tempPin=0;

void setup()
{
  /*wifi config*/
  WiFi.config(ip);
  Serial.begin(9600); //config baudrate (for testing)
  if (WiFi.status() == WL_NO_SHIELD) {Serial.println("WiFi shield not present");}
  else
  {
    while ( status != WL_CONNECTED) {status = WiFi.begin(ssid, password);}
    server.begin();
  }
  /*peltier config*/
  pinMode(tempPin, INPUT);
}
void loop()
{
  client=server.available();
  if(client)
  {
    while(client.connected())
    {
      if(client.available()>0){processClient();}
    }
    client.stop(); //when to stop?
    Serial.println("client disconnected");
  }
  //else adjust automatically?
}
void processClient()
{
  while(client.available()>0)
  {
    char message = client.read(); //store and parse message
    Serial.write(message); //do something with message for now
  }
  Serial.println();
  //this should return something
}
void sendTempData()
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    //client.println("Refresh: 3"); //should we do this??
    client.println();
    client.println("<DOCTYPE HTML>");
    client.println("<html>");
    client.print("peltier temp: ");
    client.print(getPeltierTemp());
    client.println("<br/>");
    client.print("outside temp: ");
    client.print(getOutsideTemp());
    client.print("<br/>");
    client.println("</html>");
}
void setPeltierTemp()
{
}
short getPeltierTemp()
{
}
short getOutsideTemp()
{
}
short computePeltierTemp()
{
  float temperature = analogRead(tempPin); //casting input to integer
  temperature = temperature * 4.8828125;
  temperature = (temperature - 500)/10.0;
  temperature = temperature*1.8 + 32;
  temperature -= 8; // Calibration
  return (short)temperature;
}
/*print the SSID of the network you're attached to*/
void printWifiStatus() 
{
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
