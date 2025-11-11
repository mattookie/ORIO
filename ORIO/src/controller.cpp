#include <WiFi.h>
#include <WebServer.h>
#include <webpage.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

bool RARight = false;
bool RALeft = false;
bool RAStop = true;
bool DECRight = false;
bool DECLeft = false;
bool DECStop = true;

const int ledPin = 2;

void setup(){
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    Serial.print("connecting to network");
    WiFi.begin(ssid, password);
    IPAddress IP = WiFi.localIP();
    Serial.print("IP address: ");
    Serial.println(IP);
    server.begin();
    // Define HTTP request handlers
    setupRequestHandlers();


}

void loop(){
    server.handleClient();
    
    if (RARight && !RAStop){
        digitalWrite(ledPin, HIGH);
    }
    if (RALeft && !RAStop){
        digitalWrite(ledPin, HIGH);
    }
    if (RAStop){
        digitalWrite(ledPin, LOW);
    }

    if (DECRight && !DECStop){
        digitalWrite(ledPin, HIGH);
    }
    if (DECLeft && !DECStop){
        digitalWrite(ledPin, HIGH);
    }
    if (DECStop){
        digitalWrite(ledPin, LOW);
    }

}

void setupRequestHandlers(){
    server.on("/", HTTP_GET, [](){
      // This sends the HTML_CONTENT string defined above to the client
      server.send(200, "text/html", webpageCode); 
    });
    // Right ascension handlers
    server.on("/RARight", HTTP_PUT, [](){ 
        RARight = true;
        RALeft = false;
        RAStop = false;
        server.send(200, "text/plain", "RA Right");
    });
    server.on("/RALeft", HTTP_PUT, [](){ 
        RARight = false;
        RALeft = true;
        RAStop = false;
        server.send(200, "text/plain", "RA Left");
    });
    server.on("/RAStop", HTTP_PUT, [](){ 
        RARight = false;
        RALeft = false;
        RAStop = true;
        server.send(200, "text/plain", "RA Stop");
    });

    // --- Declination (DEC) Handlers ---
    server.on("/DECRight", HTTP_PUT, [](){ 
        DECRight = true;
        DECLeft = false;
        DECStop = false;
        server.send(200, "text/plain", "DEC Right");
    });
    server.on("/DECLeft", HTTP_PUT, [](){ 
        DECRight = false;
        DECLeft = true;
        DECStop = false;
        server.send(200, "text/plain", "DEC Left");
    });
    server.on("/DECStop", HTTP_PUT, [](){ 
        DECRight = false;
        DECLeft = false;
        DECStop = true;
        server.send(200, "text/plain", "DEC Stop");
    });
}