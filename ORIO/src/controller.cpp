#include <WiFi.h>
#include <WebServer.h>
#include <webpage.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

char XML[2048];

//buffer holder for char operations
char buf[32];

void setup(){
    Serial.begin(9600);

    // disable watchdog timer because it is annoying says guy in video
    disableCore1WDT();


}

