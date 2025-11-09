#include <WiFi.h>
#include <WebServer.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

const char* HTML_CONTENT = R"rawliteral(
<!DOCTYPE html>

<!-- This contains the meta data about the document -->
<head>
    <meta charset = "UTF-8">

    <!-- This makes the website viewable on a mobile screen opposed to a desktop -->
    <

    <title> ORIO Star Tracker</title>



</head>


)rawliteral";