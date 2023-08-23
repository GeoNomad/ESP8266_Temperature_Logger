//  DS18b20 Temperature logger to server
//  MIT License benlo.com
//  Low power version sleeps between reports
//  Serial debug info is commented out

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

WiFiClient client;
int status = WL_IDLE_STATUS;
char server[] = "yourserver.com";
float temp;

void setup()
  {
  //Serial.begin(115200);
  sensors.begin();
 // Serial.println("start");
 
  WiFi.begin("your ssid", "your pw");

  //Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
  //  Serial.print(".");
    }
  //Serial.println();

  //Serial.print("Connected, IP address: ");
  //Serial.println(WiFi.localIP());
  //printWifiStatus();
  if (client.connect(server, 80)) 
    {
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    String ptr = "GET /temp.php?L2=";
    ptr += temp;
    ptr += " HTTP/1.1";
    client.println(ptr);
    client.println("Host: yourhost.com");
    client.println("Connection: close");
    client.println();
    //Serial.println(temp);
    }
  }
int i;
void loop() 
  {
  while (client.available()) 
    {
    char c = client.read();
    //Serial.write(c);
    }
  if (!client.connected()) 
    {
    //Serial.println();
    //Serial.println("disconnected.");
    ESP.deepSleep(30e7); // 5 minutes = 30e7
    }
 }
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
