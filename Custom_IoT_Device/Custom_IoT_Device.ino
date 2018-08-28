#include <CallbackFunction.h>
#include <Switch.h>
#include <UpnpBroadcastResponder.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

// prototypes
boolean connectWifi();

//on/off callbacks 
void lightOn();
void lightOff();

// Change this before you flash
const char* ssid = "AllYourBaseAreBelongToUs";
const char* password = "weAreNinjas";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *light = NULL;

//relay pin setup for funct
int relay = D2;
int button = D3;
bool LightOn = false;

void setup()
{
 Serial.begin(74880);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    light = new Switch("Light", 80, lightOn, lightOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*light);
    
    //relay pins setup i Used D1,D2,D3,D4,D5,D6,D7,D8 followed as assigned below, if you are willing to change Pin or planning to use extra please Check Image in Github File..:)
    pinMode(relay, OUTPUT);
    pinMode(button, INPUT);
    digitalWrite(relay, LOW);
  }
}
 
void loop()
{
   if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      light->serverLoop();
   }
   if(digitalRead(button) == LOW){
    while(digitalRead(button) != HIGH){
      delay(100);
    }
    if(LightOn){
      lightOff();
    }
    else{
      lightOn();
    }
  }
}

void lightOff() {
    Serial.print("Switch 1 turn off ...");
    digitalWrite(relay, LOW);   // sets relayOne off
    LightOn = false;
}

void lightOn() {
    Serial.print("Switch 1 turn on ...");
    digitalWrite(relay, HIGH);   // sets relayOne on
    LightOn = true;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
