#include <WiFi.h>
#include <WebServer.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

const char* HTML_CONTENT = R"rawliteral(