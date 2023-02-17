#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//----------------------------------------Include the Servo Library
#include <Servo.h>

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
#define ServoPort D0   //--> Defining Servo Port
const int led=D6;
//----------------------------------------SSID and Password of your WiFi router
const char *ssid = "EL KENZ";
const char *password = "@mohamed1@";
//----------------------------------------
IPAddress staticIP( 192,168,100,20); //ESP static ip
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress gateway(192,168,100,1);   //IP Address of your WiFi Router (Gateway)
IPAddress dns(8, 8, 8, 8);  //DNS 
Servo myservo;  //--> create servo object to control a servo
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

ESP8266WebServer server(80);  //--> Server on port 80

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//----------------------------------------

//----------------------------------------Procedure for handling servo control
void handleServo(){
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(1);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane","");
}
//----------------------------------------
//----------------------------------------Setup----------------------------------------
void setup() {
  myservo.write(90);
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board

  myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo object
   WiFi.config(staticIP, subnet, gateway, dns );
  WiFi.begin(ssid, password);

  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only)
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------
disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
  {
     Serial.println("Station disconnected ");
     digitalWrite(led , HIGH);
     Serial.print("reconnecting....!");
     WiFi.reconnect();
     Serial.print(".");
  });
  
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
  {
    Serial.println("Station connected again");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
    digitalWrite(led , LOW);
    Serial.println("HTTP server started agein");
  });
  //----------------------------------------Initialize Webserver
  server.on("/",handleRoot);  //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS",handleServo); //--> Sets servo position from Web request
  server.begin();  
  Serial.println("HTTP server started");
}
//------------------------------------------------------------------------------------
//----------------------------------------Loop----------------------------------------
void loop() {
 server.handleClient();
}
//------------------------------------------------------------------------------------
