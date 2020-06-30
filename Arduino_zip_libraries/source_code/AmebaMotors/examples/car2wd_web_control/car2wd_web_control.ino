/*
 * This sketch shows how to control car on web page (via http protocol)
 *
 * At first we connect device to AP, start a web server at port 80.
 * In client side we input device's IP at browser, and a page is show up.
 * There are several buttons which can control the car.
 * As we type one of these buttons, a request is append at the hyper link.
 * And device parse the link and do correspond actions.
 * 
 * The web control part reference from http://www.instructables.com/id/Internet-Controlled-RC-Car/
 */

#include <WiFi.h>
#include "Car2wd.h"

char ssid[] = "yourNetwork";      // your network SSID (name)
char pass[] = "secretPassword";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

Car2wd car(8,9,10,11,12,13);

void setup() {
  car.begin();

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) {
      break;
    }
    // wait 10 seconds for connection:
    delay(10000);
  }

  server.begin();
}

char buffer[1024];
void loop() {
  WiFiClient client = server.available();
  while (client.connected()) {
    int n = client.read((uint8_t*)(&buffer[0]), sizeof(buffer));

    client.println("HTTP/1.1 200 OK"); 
    client.println("Content-Type: text/html");
    client.println();
    client.println("<HTML>");
    client.println("<HEAD>");
    client.println("<TITLE>Internet Controlled 2WD Car</TITLE>");
    client.println("<STYLE>");
    client.println("body{margin:50px 0px; padding:0px; text-align:center;}");
    client.println("h1{text-align: center; font-family:\"Trebuchet MS\",Arial, Helvetica, sans-serif; font-size:24px;}");
    client.println("a{text-decoration:none; width:75px; height:50px; border-color:black; font-family:\"Trebuchet MS\",Arial, Helvetica, sans-serif; padding:6px; background-color:#aaaaaa; text-align:center; border-radius:10px 10px 10px; font-size:24px;}");
    client.println("a:link {color:white;}");
    client.println("a:visited {color:white;}");
    client.println("a:hover {color:red;}");
    client.println("a:active {color:white;}");
    client.println("</STYLE>");
    client.println("</HEAD>");
    client.println("<BODY>");
    client.println("<H1>Internet Controlled 2WD Car</H1>");
    client.println("<br />");
    client.println("<br />");        
    client.println("<a href=\"/?forwardLeft\"\">FORWARD LEFT</a>");
    client.println(" ");
    client.println("<a href=\"/?forward\"\">FORWARD</a>");  
    client.println(" ");      
    client.println("<a href=\"/?forwardRight\"\">FORWARD RIGHT</a>");
    client.println("<br />");
    client.println("<br />");
    client.println("<br />");
    client.println("<a href=\"/?stop\"\">STOP</a>");
    client.println("<br />");
    client.println("<br />");
    client.println("<br />");
    client.println("<a href=\"/?backwardLeft\"\">BACKWARD LEFT</a>");
    client.println(" ");
    client.println("<a href=\"/?backward\"\">BACK</a>");
    client.println(" ");  
    client.println("<a href=\"/?backwardRight\"\">BACKWARD RIGHT</a>");
    client.println("</BODY>");
    client.println("</HTML>");

    //Stop loading the website     
    delay(1);
    client.stop();

    if(strstr(buffer, "?stop") != NULL) {
      car.stop();
    } else if(strstr(buffer, "?forwardLeft") != NULL) {
      car.setAction(CAR_FORWARD_LEFT);
    } else if(strstr(buffer, "?forwardRight") != NULL) {
      car.setAction(CAR_FORWARD_RIGHT);
    } else if(strstr(buffer, "?backwardLeft") != NULL) {
      car.setAction(CAR_BACKWARD_LEFT);
    } else if(strstr(buffer, "?backwardRight") != NULL) {
      car.setAction(CAR_BACKWARD_RIGHT);
    } else if(strstr(buffer, "?forward") != NULL) {
      car.setAction(CAR_FORWARD);
    } else if(strstr(buffer, "?backward") != NULL) {
      car.setAction(CAR_BACKWARD);
    }
  }
}