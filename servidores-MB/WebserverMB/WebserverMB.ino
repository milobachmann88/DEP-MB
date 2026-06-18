#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <Servo.h>

const int servoPin= 36;

Servo servo;

const char *ssid = "PEINE-3";          // Change this to your WiFi SSID
const char *password = "etecPeine3";  // Change this to your WiFi password
const char *endpoint = "/api/servo";
const int port= 6000;

WebServer server(port);

void recibirServo(){

  Serial.println("Se recibió una conexión");
  String body = server.arg("plain");
  Serial.println("Contenido: ");
  Serial.println(body);
  JSonDocument doc;
  deserializeJson(doc, body);
  int angulo= doc["angulo"];
  Serial.print("angulo: ");
  Serial.println(angulo);
  servo.write(angulo);

  server.send(200, "text/html", "<h1>OK</h1>");
}

void noEncontrada(){
  Serial.println("Se recibió una conexión desconocida");
  server.send(404, "text/html", "<h1>Ruta desconocida, corra en /api/servo</h1>");
}

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  servo.write(0);
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
  Serial.print("Servidor corriendo en puerto: ");
  Serial.println(port);

  server.on("/api/servo", HTTP_POST, recibirServo);
  server.onNotFound(noEncontrada);
  //puedo configurar otras rutas
  server.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    server.handClient();
  }
  else{
    Serial.print("Se desconectó el WiFi");
  }
  delay(5000);
}