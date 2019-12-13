#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include "BaseProtocol.h"
#include "FSMprotocol.h"

// Definimos el puerto en el modulo Esp8266 para
// realizar la comunicacion con la placa EDU-CIAA
SoftwareSerial CIAA(D2,D3); //Rx, Tx

// Actualícelos con valores de su red WIFI

//Parametros conexión WIFI
const char *ssid = "WiFi-Arnet-11dv";
const char *password = "TKCWTYY7LP";

//Parametros conexión servidor MQTT
const char *mqtt_server = "tailor.cloudmqtt.com";
const int   mqttPort = 17750;
const char *mqttUser = "testmqtt";
const char *mqttPassword = "456789";
const char *LED_ON = "ON";
const char *LED_OFF = "OFF";
const char *TOPIC_LED = "/ciaa/led";
const char *TOPIC_TECLA = "/ciaa/pulsador";
const char *BUTTON_UP = "No Presionado";
const char *BUTTON_DOWN = "Presionado";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Permite la conexion con la placa EDU-CIAA
// utilizando un protocolo de comunicación
FSMprotocol *fsmProtocol = new FSMprotocol();

/*
*   Configuración de la conexion WIFI
*/
void setup_wifi()
{

  delay(10);
  // Inicio conexión a una red WiFi
  Serial.println();
  Serial.print("Conectado a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 *   Publicar un topic MQTT
*/
void sendMessage(bool status)
{
  if (status)
  {
    client.publish(TOPIC_LED, LED_ON);
    client.publish(TOPIC_TECLA, BUTTON_DOWN);
  }
  else
  {
    client.publish(TOPIC_LED, LED_OFF);
    client.publish(TOPIC_TECLA, BUTTON_UP);
  }
}

void reconnect()
{
  // Se ejecuta hasta que estemos reconectados
  while (!client.connected())
  {
    Serial.println("Estableciendo conexion servidor MQTT");
    // Verificamos la conexion
    if (client.connect("ESP8266Client", mqttUser, mqttPassword ))
    {
      Serial.println("conectado");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.println(" se reconectara en 5 segundo");
      // Esperamos 5 segundos para volver a intetar
      delay(5000);
    }
  }
}

/*
*   Obtiene el evento desde la placa EDU-CIAA
*/
void event_ciaa(int led)
{
  Serial.println(led);
  switch (led)
  {
  case LED_1_OFF:
    sendMessage(false);
    break;
  case LED_1_ON:
    sendMessage(true);
    break;
  }
  Serial.println("event MQTT");
}

/*
*   Ejecuta el evento desde el servidor MQTT
*/
void mqtt_event_led(uint8_t event)
{
  switch (event)
  {
  case LED_1_OFF:
    fsmProtocol->send(LED_1_OFF);
    break;
  case LED_1_ON:
    fsmProtocol->send(LED_1_ON);
    break;
   }
   Serial.println("event EDU-CIAA");  
}
  /*
  *   Verifica el evento desde el servidor MQTT
  */
  void processMessage(char *payload)
  {
    if (strcmp(payload, LED_ON) == 0)
    {
       mqtt_event_led(LED_1_ON);
       Serial.println("LED_ON");
    }

    if (strcmp(payload, LED_OFF) == 0)
    {
       mqtt_event_led(LED_1_OFF);
       Serial.println("LED_OFF");
    }     
  }

 /*
  *   Obtiene el evento desde el servidor MQTT
  */
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido ");
  Serial.println(topic);
  char mesagge[length];

  for (int i = 0; i < length; i++)
  {
    mesagge[i] = (char)payload[i];
  }
  Serial.println(*mesagge);
  processMessage(mesagge);
}

  void setup()
  {
    Serial.begin(115200);
    //Seteamos la funcion que recibira los eventos de la EDU-CIAA
    fsmProtocol->function_callback(&event_ciaa);    
    CIAA.begin(115200);
    pinMode(D2, INPUT);
    pinMode(D3, OUTPUT);
    //Seteamos el Serial por el que se comunicara 
    //el esp8266 con la EDU-CIAA
    fsmProtocol->setSerial(&CIAA);
    setup_wifi();
    client.setServer(mqtt_server, mqttPort);    
    client.setCallback(callback);
  }

  void loop()
  {
    fsmProtocol->loop();
    if (!client.connected())
    {
      reconnect();
    }
    client.loop();    
    client.subscribe(TOPIC_LED);
  }
