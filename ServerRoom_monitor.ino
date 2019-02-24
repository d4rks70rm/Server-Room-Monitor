#include <Adafruit_Sensor.h>
#include <rBase64.h>
//#include <PubNub.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>

const char* ssid     = "SSID";
const char* password = "SSID KEY";

const char* host = "SERVER URL";
const int httpPort = 80;

int station_id = 1;
String hash = "HASH";

float t = 0;
float h = 0;

String url_begin = "/URL.php?data=";
String url_id = "{\"id\":";
String url_hash =  ",\"hash\":";
String url_sensor = ",\"sensor\":\"";
String url_tempSensor = "\",\"temp\":";
String url_humSensor = ",\"hum\":";
String url_end = "}";

#define DHTPIN1  D4
#define DHTPIN2  D5
#define DHTPIN3  D7


#define DHTTYPE DHT22  

DHT_Unified dht1(DHTPIN1, DHTTYPE);
DHT_Unified dht2(DHTPIN2, DHTTYPE);
DHT_Unified dht3(DHTPIN3, DHTTYPE);

sensors_event_t event;

WiFiClient wclient;

void setup() {
  Serial.begin(9600); 
  dht1.begin();
  dht2.begin();
  dht3.begin();

  sensor_t sensor1;
  sensor_t sensor2;
  sensor_t sensor3;
  dht1.temperature().getSensor(&sensor1);
  dht2.temperature().getSensor(&sensor2);
  dht3.temperature().getSensor(&sensor3);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {

      String data1 = url_id + "\"" + station_id + "\"" + url_hash + "\"" + hash + "\"" + url_sensor + "1" + url_tempSensor + "\"" + readTemperature( dht1 ) + "\"" + url_humSensor + "\"" + readHumidity( dht1 ) + "\"" + url_end;
      String data2 = url_id + "\"" + station_id + "\"" + url_hash + "\"" + hash + "\"" + url_sensor + "2" + url_tempSensor + "\"" + readTemperature( dht2 ) + "\"" + url_humSensor + "\"" + readHumidity( dht2 ) + "\"" + url_end;
      String data3 = url_id + "\"" + station_id + "\"" + url_hash + "\"" + hash + "\"" + url_sensor + "3" + url_tempSensor + "\"" + readTemperature( dht3 ) + "\"" + url_humSensor + "\"" + readHumidity( dht3 ) + "\"" + url_end;
      
      sendResults(data1);
      delay(500);
      sendResults(data2);
      delay(500);
      sendResults(data3);
      delay(500);

  delay(1800000);

}
//function to send results to the server
void sendResults(String data){
  // Use WiFiClient class to create TCP connections
  if (!wclient.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  rbase64.encode(data);  
  // This will send the request to the server
  wclient.print(String("GET ") + url_begin + rbase64.result() + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");  
}

//function to read temperature from sensor
float readTemperature(DHT_Unified dht){
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    return 0.00;
  }
  else {
    return event.temperature;
  }    
}

//function to read humidity from sensor
float readHumidity(DHT_Unified dht){
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    return 0.00;
  }
  else {
    return event.relative_humidity;
  }  
}

