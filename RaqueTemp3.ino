#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Esse programa realiza gerenciamento de lampadas e medicao de temperatura da sala.
 
const char* ssid = "Paulo 1";
const char* password = "Joca0505";
int status = WL_IDLE_STATUS;
WiFiClient  client;
//int minut = 5;
//int cont = minut + 1;
unsigned long previousMillis = 0;
const long interval = 300000;
int teste = 0;
 
//WiFiServer server(80);

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 13
#define ONE_WIRE_BUS 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[6];

unsigned long myChannelNumber = 170635;
const char * myWriteAPIKey = "RZSK1Q6HS97Q3WRU";

 
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Iniciando programa RaqueTemp.");

  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

  delay(100);
  // prepare GPIOs
  pinMode(14, OUTPUT); //Spot
  digitalWrite(14, 0);
 
  delay(100);
  pinMode(5, OUTPUT); //Luz indireta
  digitalWrite(5, 0);
  delay(100);

  
  // Connect to WiFi network
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
  
  // Start the server
/*  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
*/
  ThingSpeak.begin(client);
}

void getTemperature() {
  float tempC;
  do {
    Serial.println("Iniciando termometro.");
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    delay(100);
    Serial.println("Temperatura:");
    Serial.println(tempC);
  } while (tempC == 85.0 || tempC == (-127.0));
  Serial.println("Termometro iniciado com sucesso.");
}

 
void loop() {
  unsigned long currentMillis = millis();


    if (teste == 0) {
    teste = 1;
    getTemperature();
    ThingSpeak.writeField(myChannelNumber, 4, temperatureCString, myWriteAPIKey);}

    if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    getTemperature();
    Serial.println(temperatureCString);
    Serial.println("Enviando dados a ThingSpeak.");

    ThingSpeak.writeField(myChannelNumber, 4, temperatureCString, myWriteAPIKey);
    }

  
 /* WiFiClient client = server.available();
  if (!client) {
    return;
    delay(300);
  }
 
   Serial.println("new client");
  while(!client.available()){
    delay(300);
  }
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  delay(300);
 
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<h3> Controle de iluminacao do raque da sala</h3>";
  buf += "<p>Iluminacao indireta <a href=\"?function=Ilind_on\"><button>Ligar</button></a> <a href=\"?function=Ilind_off\"><button>Desligar</button></a></p>";
  buf += "<p>Spot <a href=\"?function=Spot_on\"><button>Ligar</button></a> <a href=\"?function=Spot_off\"><button>Desligar</button></a></p>";
  buf += "<p>Criado por Paulo Campos <a href=\"?function=Teste\"><button>Teste</button></a></a></p>";
  buf += "<h4>Casa de Paulo e Joca</h4>";
  buf += "</html>\n";

  getTemperature();
  Serial.println(temperatureCString);
  
  client.print(buf);
  client.flush();
  client.print("Temperatura: ");
  client.print(temperatureCString);
  client.print(" ºC");
 
  if (req.indexOf("Ilind_off") != -1)
    digitalWrite(5, 1);
  else if (req.indexOf("Ilind_on") != -1)
    digitalWrite(5, 0);
  else if (req.indexOf("Spot_off") != -1)
    digitalWrite(14, 1);
  else if (req.indexOf("Spot_on") != -1)
    digitalWrite(14, 0);
  else if (req.indexOf("Teste") != -1)
    Serial.println(req.indexOf("Teste"));
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("Client disonnected");
*/
}
