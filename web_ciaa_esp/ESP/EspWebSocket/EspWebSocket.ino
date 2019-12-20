#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>
#include "FSMprotocol.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "BaseProtocol.h"


SoftwareSerial CIAA(D2,D3); //Rx, Tx
#define DBG_OUTPUT_PORT Serial
#define BODY_FORMAT "{\"led\":%d,\"on\":%d}"

IPAddress ip(192,168,1,102);     
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0);

const char *ssid = "WiFi-Arnet-11dv";
const char *password = "TKCWTYY7LP";
const char *host = "esp8266sd";
const int chipSelect = D1;

ESP8266WebServer server(80);

// Globals
WebSocketsServer webSocket = WebSocketsServer(81);

FSMprotocol *fsmProtocol = new FSMprotocol();


void sendObject(int _led, int _on)
{
    StaticJsonDocument<30> doc;
    String ledJson;
    doc["led"] = _led;
    doc["on"] = _on;
 
    serializeJson(doc, ledJson);    
    Serial.println(ledJson);
    webSocket.broadcastTXT(ledJson);
}
 

void event_led(int led)
{  
  Serial.println(led); 
  switch (led)
  {
  case LED_1_OFF:
    //sprintf(json, BODY_FORMAT, 1,  0); 
    sendObject(1,0);
    break;
  case LED_1_ON:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(1,1);
    break;
  case LED_2_OFF:
    //sprintf(json, BODY_FORMAT, 1,  1);  
    sendObject(2,0);  
    break;
  case LED_2_ON:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(2,1);
    break;
  case LED_3_OFF:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(3,0);
    break;
  case LED_3_ON:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(3,1);
    break;
  case LED_4_OFF:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(4,0);
    break;
  case LED_4_ON:
    //sprintf(json, BODY_FORMAT, 1,  1);    
    sendObject(4,1);
    break;
  }  
  Serial.println("event_led"); 
}

void ciaa_event_led(uint8_t event)
{
 switch (event)
  {
  case LED_1_OFF:
    fsmProtocol->send(LED_1_OFF);
    break;
  case LED_1_ON:
    fsmProtocol->send(LED_1_ON);
    break;
  case LED_2_OFF:
    fsmProtocol->send(LED_2_OFF);
    break;
  case LED_2_ON:
    fsmProtocol->send(LED_2_ON);
    break;
  case LED_3_OFF:
    fsmProtocol->send(LED_3_OFF);
    break;
  case LED_3_ON:
    fsmProtocol->send(LED_3_ON);
    break;
  case LED_4_OFF:
    fsmProtocol->send(LED_4_OFF);
    break;
  case LED_4_ON:
    fsmProtocol->send(LED_4_ON);
    break;
  }
  Serial.println("ciaa_event_led");  
}

void processMessage(uint8_t *payload){  
    StaticJsonDocument<30> doc;    
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
     
    uint8_t _led = atoi(doc["led"]);
    uint8_t _on = atoi(doc["on"]);    

    Serial.println(_led);
    Serial.println(_on);
    
      switch (_led)
      {
      case 1:
        if (_on)
        {
          ciaa_event_led(LED_1_ON);
        }
        else
        {
          ciaa_event_led(LED_1_OFF);
        }
        break;
      case 2:
        if (_on)
        {
          ciaa_event_led(LED_2_ON);
        }
        else
        {
          ciaa_event_led(LED_2_OFF);
        }
        break;
      case 3:
        if (_on)
        {
          ciaa_event_led(LED_3_ON);
        }
        else
        {
          ciaa_event_led(LED_3_OFF);
        }
        break;
      case 4:
        if (_on)
        {
          ciaa_event_led(LED_4_ON);
        }
        else
        {
          ciaa_event_led(LED_4_OFF);
        }
        break;
      }
}

// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{

  // Figure out the type of WebSocket event
  switch (type)
  {

  // Client has disconnected
  case WStype_DISCONNECTED:
    //Serial.printf("[%u] Disconnected!\n", num);
    break;

  // New client has connected
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connection from ", num);
    Serial.println(ip.toString());
  }
  break;

  // Echo text message back to client
  case WStype_TEXT:
    processMessage(payload);    
    //Serial.println(*payload);
    break;
  }
}

static bool hasSD = false;
File uploadFile;

void returnOK()
{
  server.send(200, "text/plain", "");
}

void returnFail(String msg)
{
  server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path)
{
  String dataType = "text/html";
  if (path.endsWith("/"))
    path += "index.html";

  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".js"))
    dataType = "application/javascript";
  else if (path.endsWith(".png"))
    dataType = "image/png";
  else if (path.endsWith(".svg"))
    dataType = "image/svg+xml";  
  else if (path.endsWith(".gif"))
    dataType = "image/gif";
  else if (path.endsWith(".jpg"))
    dataType = "image/jpeg";
  else if (path.endsWith(".ico"))
    dataType = "image/x-icon";
  else if (path.endsWith(".xml"))
    dataType = "text/xml";
  else if (path.endsWith(".zip"))
    dataType = "application/zip";

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory())
  {
    path += "/index.html";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download"))
    dataType = "application/octet-stream";

  if (server.streamFile(dataFile, dataType) != dataFile.size())
  {
    DBG_OUTPUT_PORT.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleFileUpload()
{
  if (server.uri() != "/edit")
    return;
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    if (SD.exists((char *)upload.filename.c_str()))
      SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    DBG_OUTPUT_PORT.print("Upload: START, filename: ");
    DBG_OUTPUT_PORT.println(upload.filename);
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {

    if (uploadFile)
      uploadFile.write(upload.buf, upload.currentSize);
    DBG_OUTPUT_PORT.print("Upload: WRITE, Bytes: ");
    DBG_OUTPUT_PORT.println(upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {

    if (uploadFile)
      uploadFile.close();
    DBG_OUTPUT_PORT.print("Upload: END, Size: ");
    DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}

void deleteRecursive(String path)
{
  File file = SD.open((char *)path.c_str());
  if (!file.isDirectory())
  {
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true)
  {
    File entry = file.openNextFile();
    if (!entry)
      break;
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory())
    {
      entry.close();
      deleteRecursive(entryPath);
    }
    else
    {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete()
{
  if (server.args() == 0)
    return returnFail("BAD ARGS");
  String path = server.arg(0);
  if (path == "/" || !SD.exists((char *)path.c_str()))
  {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate()
{
  if (server.args() == 0)
    return returnFail("BAD ARGS");
  String path = server.arg(0);
  if (path == "/" || SD.exists((char *)path.c_str()))
  {
    returnFail("BAD PATH");
    return;
  }

  if (path.indexOf('.') > 0)
  {
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if (file)
    {
      file.write((const char *)0);
      file.close();
    }
  }
  else
  {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory()
{
  if (!server.hasArg("dir"))
    return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str()))
    return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory())
  {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt)
  {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]");
  dir.close();
}

void handleNotFound()
{
  if (hasSD && loadFromSdCard(server.uri()))
    return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  DBG_OUTPUT_PORT.print(message);
}

void setup(void)
{   
  fsmProtocol->function_callback(&event_led);
  DBG_OUTPUT_PORT.begin(115200);
  CIAA.begin(115200);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(SS, OUTPUT);
  fsmProtocol->setSerial(&CIAA); 
  DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.print("\n");
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  DBG_OUTPUT_PORT.print("Connecting to ");
  DBG_OUTPUT_PORT.println(ssid);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20)
  { //wait 10 seconds
    delay(500);
  }
  if (i == 21)
  {
    DBG_OUTPUT_PORT.print("Could not connect to");
    DBG_OUTPUT_PORT.println(ssid);
    while (1)
      delay(500);
  }
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  if (MDNS.begin(host))
  {
    MDNS.addService("http", "tcp", 80);
    DBG_OUTPUT_PORT.println("MDNS responder started");
    DBG_OUTPUT_PORT.print("You can now connect to http://");
    DBG_OUTPUT_PORT.print(host);
    DBG_OUTPUT_PORT.println(".local");
  }

  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, []() { returnOK(); }, handleFileUpload);
  server.onNotFound(handleNotFound);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
  if (SD.begin(chipSelect))
  {
    DBG_OUTPUT_PORT.println("SD Card initialized.");
    hasSD = true;
  }else{
    DBG_OUTPUT_PORT.println("SD Card not initialized.");
  }

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop(void)
{  
  // Look for and handle WebSocket data
  fsmProtocol->loop();
  webSocket.loop();
  server.handleClient();    
}
