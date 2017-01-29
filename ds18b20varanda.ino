#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Replace with your network details
const char* ssid = "LAB_ABERTO";
const char* password = "C1T@C3TEC";
int status = WL_IDLE_STATUS;
WiFiClient  client;


// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 14
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[6];
char temperatureFString[6];

// Web Server on port 80
WiFiServer server(80);

unsigned long myChannelNumber = 170635;
const char * myWriteAPIKey = "RZSK1Q6HS97Q3WRU";

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  
  // Connecting to WiFi network
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
  
// only runs once on boot
  
  ThingSpeak.begin(client);
}

void getTemperature() {
  float tempC;
  float tempF;
  do {
    Serial.println("1");
    DS18B20.requestTemperatures();
    Serial.println("2"); 
    tempC = DS18B20.getTempCByIndex(0);
    Serial.println("3");
    dtostrf(tempC, 2, 2, temperatureCString);
    Serial.println("4");
    tempF = DS18B20.getTempFByIndex(0);
    Serial.println("5");
    dtostrf(tempF, 3, 2, temperatureFString);
    Serial.println("6");
    delay(100);
    Serial.println("Temperatura:");
    Serial.println(tempC);
  } while (tempC == 85.0 || tempC == (-127.0));
}

void loop() {

  getTemperature();
  Serial.println(temperatureCString);

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeak.writeField(myChannelNumber, 2, temperatureCString, myWriteAPIKey);
  delay(20000);
  delay(20000);
  delay(20000);// ThingSpeak will only accept updates every 15 seconds.
}
