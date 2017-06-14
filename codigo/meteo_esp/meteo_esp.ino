
// LCD I2C

#include <Wire.h> 
#include <LiquidCrystal_I2C_ESP.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C_ESP lcd(0x27, 16, 2);

// Sensor DHT 

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            D1         // Pin which is connected to the DHT sensor.

#define DHTTYPE           DHT22     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS; // Tiempo recomendado entre medidas

double temperature,humedad;
double CO_ppm;


// Wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "TU_SSID";
const char *password = "TU_CONTRASENA";

ESP8266WebServer server ( 80 );

const int led = LED_BUILTIN;

#define BUFFER_SIZE 1000

void handleRoot() {
  digitalWrite ( led, 1 );
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, BUFFER_SIZE,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Temperatura: %d.%d C  </br> \
    Humedad: %d %  </br> \
    Concentracion de CO: %d.%d ppm</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

    hr, min % 60, sec % 60,
    int(temperature),int(temperature*100-int(temperature)*100),
    int(humedad),
    int(CO_ppm),int(CO_ppm*100-int(CO_ppm)*100)
  );
  server.send ( 200, "text/html", temp );
  digitalWrite ( led, 0 );
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x+= 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}


void setup() {
  Serial.begin ( 115200 );

  // DHT 
  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  // initialize the LCD
  lcd.begin(D3,D4);  // sda=0 | D3, scl=2 | D4

  lcd.backlight();

  // Wifi

  pinMode ( led, OUTPUT );
  digitalWrite ( led, 0 );

  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/test.svg", drawGraph );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

}

double CO_PPM() {
  int CO_Raw=analogRead(A0);
  double CO_PPM=3.027*exp(1.0698*(CO_Raw*3.3/1023));
    
}


void readDHT(){
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    temperature=event.temperature;
    Serial.print(temperature);
    Serial.println(" *C");
    lcd.setCursor(0,0);
    lcd.print(temperature);
    lcd.print("*C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    humedad=event.relative_humidity;
    Serial.print(humedad);
    Serial.println("%");
    lcd.setCursor(10,0);
    lcd.print(humedad);
    lcd.print("%");
  }
}


void readCO(){
   Serial.print("Gas (MQ7):");
   CO_ppm=CO_PPM();
  Serial.println(CO_ppm);
  lcd.setCursor(0,1);
  lcd.print("CO ");
  lcd.print(CO_ppm);
  lcd.print(" ppm");

}

long lastSensorRead=millis();
void loop() {
  
  server.handleClient();
  if(millis()-lastSensorRead>delayMS)
  {
    readDHT();
    
    readCO();
    
    lastSensorRead=millis();
  }
  
 
}
