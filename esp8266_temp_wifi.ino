//  DS18b20 Temperature logger to server
//  MIT License benlo.com

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2
#define analogPin A0

const short int LED = 16; //GPIO16

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

WiFiClient client;
int status = WL_IDLE_STATUS;
char server[] = "benlo.com";
float temp;
int adcValue = 0; 

void setup()
  {
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, 1);  // off

  Serial.begin(115200);
  Serial.println();
  sensors.begin();
 
  WiFi.begin("Your SSID", "Your PW");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  printWifiStatus();
  if (client.connect(server, 80)) 
    {
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    String ptr = "GET /test.php?temp=";  
    ptr += temp;
    ptr += " HTTP/1.1";
    client.println(ptr);
    client.println("Host: yourserver.com");
    client.println("Connection: close");
    client.println();
    Serial.println(temp);
    }
  }
int i;
void loop() 
  {
  while (client.available()) 
    {
    char c = client.read();
    Serial.write(c);
    digitalWrite(LED, c & 1);
    }
  if (!client.connected()) 
    {
    Serial.println();
    Serial.println("disconnected.");
    while(millis() < 60e3) 
      {
      flash(1000);  // wait 60 seconds
      }
    ESP.restart();  
    }
 flash(250);
 }
void flash(int mSec)
  {
  digitalWrite(LED,1);
  delay(mSec);
  digitalWrite(LED,0);
  delay(mSec);
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
