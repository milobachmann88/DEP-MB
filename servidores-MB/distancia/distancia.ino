#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "PEINE-3";          // Change this to your WiFi SSID
const char *password = "etecPeine3";  // Change this to your WiFi password
const char *endpoint = "http://10.56.13.13:5000/api/sensor";

void setup() {
Serial.begin(115200);
pinMode(34, INPUT);
analogSetPinAttenuation(34, ADC_11db);
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
}

void loop() {
  int valorPot= analogRead(34);
if (WiFi.status() == WL_CONNECTED){
  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");

  JsonDocument doc;
  /*JsonArray nombres= doc["nombres"].to<JsonArray>();
  nombres.add("Camila");
  nombres.add("Milo");
  */
  doc["nombre"]= "potenciometro";
  doc["valor"]= valorPot;

  String payload;
  serializeJson(doc, payload);

  int httpCode= http.POST(payload);
  const int OK= 200;
  if(httpCode==OK){
    Serial.print("Datos enviados: ");
    Serial.println(payload);
    String response = http.getString();
    Serial.print("Respuesta recibida: ");
    Serial.print(response);
  }
  else{
    Serial.print("Error al conectar con el servidor: ");
    Serial.println(httpCode);
  }
  http.end();
}
else{
  Serial.print("Se desconectó el WiFi");
}
delay(5000);
}