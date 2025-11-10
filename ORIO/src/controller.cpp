#include <WiFi.h>
#include <WebServer.h>
#include <webpage.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

