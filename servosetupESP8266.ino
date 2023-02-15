
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#include "PageIndex.h";
const int led=D6;
int servo_pin = D7;  // for ESP8266 microcontroller

Servo myservo;  //--> create servo object to control a servo


//Static IP address configuration
IPAddress staticIP( 192,168,11,141); //ESP static ip
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress gateway(192,168,11,153);   //IP Address of your WiFi Router (Gateway)

IPAddress dns(8, 8, 8, 8);  //DNS
//const char* deviceName = "circuits4you.com";

//SSID and Password of your WiFi router
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
const char* ssid = "El kenz";
const char* password = "@mohamed1@";

//Declare a global object variable from the ESP8266WebServer class.   
ESP8266WebServer server(80); //Server on port 80


//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
 
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s);//Send web page
 
}
void handleservo() {
 String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane","");
}
void setup() {
    pinMode(led, OUTPUT);
  //==================================
  server.begin();      //Start server
 //==================================
  Serial.begin(115200);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
  digitalWrite(led , HIGH);
  delay(15);
  digitalWrite(led , LOW);
  delay(15);
    
//  WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
  WiFi.config(staticIP, subnet, gateway, dns );
  WiFi.begin(ssid, password);

  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
    // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(80);
    Serial.print(".");
    digitalWrite(led , HIGH);
    delay(15);
    digitalWrite(led , LOW);
    delay(15);
 }
   //If connection successful show IP address in serial monitor
  digitalWrite(led , LOW);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  Serial.println("HTTP server started");
  
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
  {
    Serial.println("Station connected again");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
    digitalWrite(led , LOW);
    Serial.println("HTTP server started agein");
  });
 
 disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
  {
     Serial.println("Station disconnected ");
     digitalWrite(led , HIGH);
     Serial.print("reconnecting....!");
     WiFi.reconnect();
     Serial.print(".");
  });

  server.on("/"  , handleRoot);
  server.on("/setPOS"  , handleservo);
}

void loop(void) {
  server.handleClient(); 
  // put your main code here, to run repeatedly:
}
