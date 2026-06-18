#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
   &Wire, OLED_RESET);

#define SDA_PIN 21
#define SCL_PIN 22


// 'agua', 20x20
const unsigned char humedad[] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00,
 0x00, 0xF0, 0x00, 0x01, 0x98, 0x00, 0x03, 0x9C, 0x00, 0x03, 0x0C, 0x00,
 0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00,
 0x0D, 0x83, 0x00, 0x0D, 0xC3, 0x00, 0x06, 0xC6, 0x00, 0x07, 0x0E, 0x00,
 0x03, 0xFC, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'term', 20x20
const unsigned char temperatura[] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x01, 0xB0, 0x00,
 0x03, 0x16, 0x00, 0x03, 0x16, 0x00, 0x03, 0x1E, 0x00, 0x03, 0x10, 0x00,
 0x03, 0x5E, 0x00, 0x03, 0xD0, 0x00, 0x03, 0xDE, 0x00, 0x03, 0xD8, 0x00,
 0x03, 0xD8, 0x00, 0x06, 0xF8, 0x00, 0x06, 0xE8, 0x00, 0x03, 0xD8, 0x00,
 0x03, 0xF0, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const char *ssid = "PEINE-3";          // Change this to your WiFi SSID
const char *password = "etecPeine3";  // Change this to your WiFi password
const char *endpoint = "/api/pantalla";
const int port= 6000;

WebServer server(port);

enum { agua, termometro }  imagen = agua;


void recibirPantalla(){

  Serial.println("Se recibió una conexión");
  String body = server.arg("plain");
  Serial.println("Contenido: ");
  Serial.println(body);
  JsonDocument doc;
  deserializeJson(doc, body);
  String dibujo= doc["dibujo"];
  if (dibujo == "agua"){
    imagen = agua;
  }
  if (dibujo == "termometro"){
    imagen = termometro;
  }
  Serial.print("dibujo: ");
  Serial.println(dibujo);

  server.send(200, "text/html", "<h1>OK</h1>");
}

void noEncontrada(){
  Serial.println("Se recibió una conexión desconocida");
  server.send(404, "text/html", "<h1>Ruta desconocida, corra en /api/pantalla</h1>");
}

void setup() {
  Serial.begin(115200);


  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("SSD1306 allocation failed"));
   for(;;); // Don't proceed, loop forever
 }
 display.clearDisplay();
 display.drawRect(10,10,30,30, SSD1306_WHITE);
  display.display();

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println("******************************************************");
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
  Serial.print("Servidor corriendo en puerto: ");
  Serial.println(port);

  server.on("/api/pantalla", HTTP_POST, recibirPantalla);
  server.onNotFound(noEncontrada);
  //puedo configurar otras rutas
  server.begin();


}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    server.handleClient();
  }
  else{
    Serial.print("Se desconectó el WiFi");
  }
  display.clearDisplay();
  if (imagen == agua){
    display.drawBitmap(10, 20, humedad, 20 , 20 , SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(35, 10);
    display.println("AGUA");
    display.setTextColor(SSD1306_WHITE);
  }

  if (imagen == termometro){
    display.drawBitmap(10, 20, temperatura, 20 , 20 , SSD1306_WHITE);


    display.setTextSize(1);
    display.setCursor(35, 10);
    display.println("TERMÓMETRO");
    display.setTextColor(SSD1306_WHITE);

  }
  display.display();
  delay(5000);
}