#include <WiFi.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// PINS SECTION

#define ForwardPin 13
#define BackwardPin 14
#define LeftPin 16
#define RightPin 17
#define SwingLeftPin 18
#define SwingRightPin 19
#define SwingUpPin 21
#define SwingDownPin 22
#define ClawOpenPin 23
#define ClawClosePin 25

// Easy Access to all pins at once
const int OutputPins[] = {ForwardPin, BackwardPin, LeftPin, RightPin, SwingLeftPin, SwingRightPin, SwingUpPin, SwingDownPin, ClawOpenPin, ClawClosePin};
const int OutputPinsCount = sizeof(OutputPins) / sizeof(OutputPins[0]);


// WIFI SECTION

const char* SSID = "MobileRoboticArm";

DNSServer DNS; 
const byte DNSPORT = 53;
AsyncWebServer Server(80);
AsyncWebSocket WS("/ws");
IPAddress IP(192,168, 69, 1);  // Server IP

unsigned long LastPingMS = 0;
const unsigned long PingInterval = 500; // In ms


// Handle the Command
void HandleCommand(uint8_t CommandByte) {

  Serial.print("Received Command 0x");
  if (CommandByte < 16) Serial.print("0");
  Serial.println(CommandByte, HEX);
  
  // Clear all pins
  for (int i = 0 ; i < OutputPinsCount; i++) {
    digitalWrite(OutputPins[i], LOW);
  }

  switch(CommandByte) { // COMMAND LIST
    
    case 0x00: // Stop
      break;

    case 0x01: // Forward
      digitalWrite(ForwardPin, HIGH);
      break;

    case 0x02: // Backward
      digitalWrite(BackwardPin, HIGH);
      break;

    case 0x03: // Left
      digitalWrite(LeftPin, HIGH);
      break;

    case 0x04: // Right
      digitalWrite(RightPin, HIGH);
      break;

    case 0x05: // SwingLeft
      digitalWrite(SwingLeftPin, HIGH);
      break;

    case 0x06: // SwingRight
      digitalWrite(SwingRightPin, HIGH);
      break;

    case 0x07: // SwingUp
      digitalWrite(SwingUpPin, HIGH);
      break;

    case 0x08: // SwingDown
      digitalWrite(SwingDownPin, HIGH);
      break;

    case 0x09: // ClawOpen
      digitalWrite(ClawOpenPin, HIGH);
      break;

    case 0x0A: // ClawClose
      digitalWrite(ClawClosePin, HIGH);
      break;

    default:
      break;

  }

}


// Handles all events in the web socket
void OnEvent(AsyncWebSocket* Server, AsyncWebSocketClient* Client, AwsEventType Type, void* Arg, uint8_t* Data, size_t Len ) {

  switch(Type) {

    case WS_EVT_CONNECT:

      if (Server->count() > 1) { // Only Allow One Connection
        Client->close();
        return;
      }

      Client->client()->setNoDelay(true); // Disable Nagle's algorithm on the client socket
      Serial.printf("Websocket Client #%u connected from %s \n", Client->id(), Client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket Client #%u disconnected \n", Client->id());
      HandleCommand( 0x00 ); // Stop Command 

      break;
    
    case WS_EVT_DATA: {
      
      AwsFrameInfo* Info = (AwsFrameInfo*)Arg;
      
      // Ensures the Info is final and complete, and is binary
      if (Info->final && Info->index == 0 && Info->len == Len && Info->opcode == WS_BINARY) {
        
        HandleCommand( Data[0] );

      }
      break;
    }

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}



void setup() {
  
  Serial.begin(115200);
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    ESP.restart();

  }
  
  // Initialize Pins
  for (int i = 0; i < OutputPinsCount; i++) {
    pinMode(OutputPins[i], OUTPUT);
    digitalWrite(OutputPins[i], LOW);

  }
  
  // Initialize Soft AP
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IP, IP, IPAddress(255,255,255,0));
  WiFi.softAP(SSID);

  DNS.start(DNSPORT, "*", IP);
  
  // Initialize and Attach WebSocket
  WS.onEvent(OnEvent);
  WS.enable(true); // Enable auto ack heartbeat 
  Server.addHandler(&WS);

  // Initialize Server Endpoints

  Server.on("/", HTTP_GET, [](AsyncWebServerRequest* Request) {
    Request->send(LittleFS, "/LandingPage.html", "text/html");
  });

  Server.on("/ControlPage", HTTP_GET, [](AsyncWebServerRequest* Request) {

    if (WS.count() > 0) { // If Force to this page without using the button
      Request->redirect("http://192.168.69.1/"); // Redirect to home

    } else {
      Request->send(LittleFS, "/ControlPage.html", "text/html");

    }
    
  });

  Server.on("/GetStatus", HTTP_GET, [](AsyncWebServerRequest* Request) {
    String JSONPayload = "{ \"IsBusy\" : " + String( WS.count() > 0 ? "true" : "false" ) + "}"; 
    Request->send(200, "application/json", JSONPayload);

  });
  

  // OS Probes
  Server.on("/generate_204", [](AsyncWebServerRequest* Request) {
    Request->redirect("http://192.168.69.1/");
  });
  Server.on("/hotspot-detect.html", [](AsyncWebServerRequest* Request) {
    Request->redirect("http://192.168.69.1/");
  });

  // Serve StaticFiles

  Server.serveStatic("/", LittleFS, "/");

  // Redirection for 404 Pages
  Server.onNotFound([](AsyncWebServerRequest* Request) {
    Request->redirect("http://192.168.69.1/");
  });

  // Start Server
  Server.begin();

  Serial.println("All Systems Ready");



}

void loop() {
  DNS.processNextRequest();

  if (millis() - LastPingMS > PingInterval) {
    WS.pingAll();
    LastPingMS = millis();
  }
  
  WS.cleanupClients();

}
