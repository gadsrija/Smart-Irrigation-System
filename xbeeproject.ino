#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "lenovo k8 note";
const char* password = "chikki09";
#include "DHT.h"
#define DHTPIN D2    
#define DHTTYPE DHT11   
DHT dht (DHTPIN, DHTTYPE);
#define ORG "f6rm4b"
#define DEVICE_TYPE "gnits.s"
#define DEVICE_ID "3113"
#define TOKEN "9121326820"
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6,D7);//rx,tx
int ldrMSB,ldrLSB,ldr;

void setup() 
{
Serial.begin(9600);
Serial.println();
dht.begin();
Serial.print("Connecting to ");
Serial.print(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
} 
Serial.println("");
Serial.print("WiFi connected, IP address: ");
Serial.println(WiFi.localIP());
mySerial.begin(9600);
pinMode(D4,OUTPUT);
}

void loop() 
{ 
int moistmsb;
int moistlsb;
int moist;
if(mySerial.available()>21){
      Serial.println("moisture data");
      
if(mySerial.read() == 0x7E){
  for (int i=1;i<=20;i++) {
    byte discardByte = mySerial.read();
   // Serial.println(discardByte);
  }
moistmsb=mySerial.read();
moistlsb=mySerial.read();
moist=moistlsb + (moistmsb * 256);
Serial.println("moist");
Serial.println(moist);
//Serial.println("moisture");
delay(2000);
}
}
float h = dht.readHumidity();
float t = dht.readTemperature();

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h,moist);
delay(1000);
if(moist>500)
{
  digitalWrite(D4,HIGH);
}
else
{
  digitalWrite(D4,LOW);
}
}

void PublishData(float temp, float humid, int moist){
if (!!!client.connected()) {
Serial.print("Reconnecting client to ");
Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
Serial.print(".");
delay(500);
 }
 Serial.println();
 }
String payload = "{\"d\":{\"temperature\":";
payload += temp;
payload+="," "\"humidity\":";
payload += humid;
payload+="," "\"moisture\":";
payload += moist;
payload += "}}";
Serial.print("Sending payload: ");
Serial.println(payload);
if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
}
else
{
 Serial.println("Publish failed");
}
}
