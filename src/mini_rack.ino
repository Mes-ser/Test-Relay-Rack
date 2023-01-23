#include <WiFi.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <PCF8574.h>
#include <ESPAsyncWebServer.h>
#include "wificonfig.h"


#ifdef mDNS
#include <ESPmDNS.h>
#endif

String inputString = "";
int LEDpin = 2;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long currentTime = 0;

PCF8574 PCF_20(0x20);
PCF8574 PCF_21(0x21);

void setup()
{   
    inputString.reserve(10);
    pinMode(LEDpin, OUTPUT);

    Serial.begin(115200);

    // Initialize mDNS
    #ifdef mDNS 
    startMDNS();
    #endif
    
    // Initialize SPIFFS
    initSPIFFS();

    // init expanders
    PCF_20.begin();
    PCF_21.begin();

    // Connect to WiFI network
    connectWiFI();

    // Start web server
    WebServer();
}

void loop()
{
    // if(millis() > currentTime + 10000){
    //     currentTime = millis();
    //     Serial.println("LifeCheck");
    // }

    if(Serial.available()){
        inputString = Serial.readStringUntil('\n');
        if(inputString == "getIP"){
            Serial.println(WiFi.localIP());
        }else{
            Serial.print("Invalid command: ");
            Serial.println(inputString);
        }
        inputString = "";
    }
}

/*** User Functions ***/
void connectWiFI(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, WIFIpassword);
    Serial.print("SSID: ");
    Serial.println(SSID);

    uint8_t i = 0;
    while( WiFi.status() != WL_CONNECTED ){
        Serial.print('.');
        delay(500);
        if( !(++i % 16) ){
            Serial.println(F("\nTrying to connect"));
        }
    }

    Serial.print(F("\nConnected. IP: "));
    Serial.println(WiFi.localIP());
}

void initSPIFFS(){
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        while(1);
    }
}

void WebServer(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });
    server.on("/control.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/control.js", "text/javascript");
    });

    server.on("/setState", HTTP_POST, [](AsyncWebServerRequest *request) {
        int args = request->args();
        for(int i=0; i<args; i++){
            String argName = request->argName(i).c_str();
            int argVal = request->arg(i).toInt();
            if(argName == "state"){
                if(argVal == 1){
                    PCF_20.write8(0xF0);
                    PCF_21.write8(0xF0);
                    digitalWrite(LEDpin,LOW);
                }else if(argVal == 0){
                    PCF_20.write8(0x0F);
                    PCF_21.write8(0x0F);
                    digitalWrite(LEDpin,HIGH);
                }
                events.send(String(digitalRead(LEDpin)).c_str(), "state");
            }
            
        }
        /*
        if(PCF_20.read(1)>0){
            events.send("1", "state");
        }else{
            events.send("0", "state");
        };*/
        request->send(200);
    });
    server.on("/bulbsSet", HTTP_POST, [](AsyncWebServerRequest *request) {
        int args = request->args();
        for(int i=0; i<args; i++){
            String argName = request->argName(i).c_str();
            int argVal = request->arg(i).toInt();
            if(argName == "state"){
                if(argVal == 1){
                    PCF_20.write8(0xF0);
                    PCF_21.write8(0xF0);
                    digitalWrite(LEDpin,LOW);
                }else if(argVal == 0){
                    PCF_20.write8(0x0F);
                    PCF_21.write8(0x0F);
                    digitalWrite(LEDpin,HIGH);
                }
                events.send(String(digitalRead(LEDpin)).c_str(), "state");
            }
            
        }

        request->send(200);
    });

    server.on("/test", HTTP_POST, [](AsyncWebServerRequest * request){
        int args = request->args();
        for(int i=0;i<args;i++){
            Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
        }
        String tekst = request->arg("states");
        Serial.println(tekst);
        int st = 0;
        for(int i=0; i < 16; i++){
            st = int(tekst[i])-48;
            Serial.print(st);
            if(i < 8){
                if(PCF_20.read(i) != st)
                    PCF_20.write(i, st);
            }else{
                if(PCF_21.read(i-8) != st)
                    PCF_21.write(i-8, st);
            }
        }
        request->send(200);
    });

    events.onConnect([](AsyncEventSourceClient *client){
        events.send(String(digitalRead(LEDpin)).c_str(), "state");
        if(client->lastId()){
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", 
                            client->lastId());
        }
        client->send("hello!",NULL,millis(),1000);
        Serial.printf("Client connected!\n");
    });
    server.addHandler(&events);

    server.onNotFound(notFound);

    server.begin();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

#ifdef mDNS
void startMDNS(){
    if (!MDNS.begin("okon")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    MDNS.addService("http", "tcp", 80);
}
#endif

void toggleLight(){

}
