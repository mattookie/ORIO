#include <WiFi.h>
#include <WebServer.h>
#include <webpage.h>

// Network Details
const char* ssid = "Matt phone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

bool RARight = false;
bool RALeft = false;
bool RAStop = true;
bool DECRight = false;
bool DECLeft = false;
bool DECStop = true;

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

void setup(){
    Serial.begin(115200);

    Serial.print("connecting to network");
    WiFi.begin(ssid, password);
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connection Successful");
    } else {
        // This is what you should see on failure in the latest version:
        Serial.println("Failed to connect to Wi-Fi.");
    }
    IPAddress IP = WiFi.localIP();
    Serial.print("IP address: ");
    Serial.println(IP);
    server.begin();
    // Define HTTP request handlers
    setupRequestHandlers();
}

void loop(){
    server.handleClient();

    if (RARight){
        Serial.println("RA Moving to Right");
    }
    if (RALeft){
        Serial.println("RA Moving to Left");
    }
    if (RAStop && DECStop){
        Serial.println("RA and DEC Stopped");
    }
    if (DECRight){
        Serial.println("DEC Moving to Right");
    }
    if (DECLeft){
        Serial.println("DEC Moving to Left");
    }

}