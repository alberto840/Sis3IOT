// Importamos las librerías
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
// Sustituye los datos de tu red WIFI ( el nombre y la contraseña )
#include "config.h"

// Puesta de LED GPIO
const int ledPin = D2;
const int ledPin2 = D3;
// Para guardar el estado del LED
String ledState;

// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Remplazamos el marcador con el estado del  LED
String processor(const String& var)
{
    Serial.print(var+" LED: ");
    //esta función primero verifica si el marcador de posición es el ESTADO que hemos creado en el archivo HTML.
    if(var == "ESTADO")
    {   //Si lo está, entonces, de acuerdo con el estado del LED, ponemos la variable ledState en ON u OFF.
        if(digitalRead(ledPin))
            { ledState = "ON"; }
        else{
           ledState = "OFF";  }
        if(digitalRead(ledPin2))
            { ledState = "ON1"; }
        else{
           ledState = "OFF1";  }
       // Imprimimos el estado del led ( por el COM activo )
       //Finalmente, se devuelve la variable ledState. Esto reemplaza el marcador de posición STATE con el valor de cadena ledState.
       Serial.println(ledState);
       return ledState;
    }
}
void setup()
{
  // Establecemos la velocidad de conexión por el puerto serie
  Serial.begin(115200);
  // Ponemos a  ledPin  como salida
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Iniciamos  SPIFFS
  if(!SPIFFS.begin())
     { Serial.println("ha ocurrido un error al montar SPIFFS");
       return; }
  // conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      // Esperamos un segundo
      Serial.println("Conectando a la red WiFi..");
    }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());
  
  // Ruta para cargar el archivo index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // Ruta para poner el GPIO alto
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(ledPin, HIGH);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // Ruta para poner el GPIO bajo
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(ledPin, LOW);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
 // Ruta para poner el GPIO alto
  server.on("/on1", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(ledPin2, HIGH);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // Ruta para poner el GPIO bajo
  server.on("/off1", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(ledPin2, LOW);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // Start server
  server.begin();
}

void loop(){

}
