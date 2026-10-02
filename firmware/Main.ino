#include <WiFi.h>
#include <time.h>
#include <ESP32Servo.h>
#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"




const char* ssid = "dushyant";
const char* pass = "12345678";+
const char* ntpServer = "pool.ntp.org";


const long gmtOffset_sec = 5 * 3600 + 30 * 60;
const int   daylightOffset_sec = 0;



const byte RXD2 = 16; 
const byte TXD2 = 17;
HardwareSerial dfSD(2);
DFRobotDFPlayerMini player;




#define BOOT_PIN 0
int lastHour = -1;



void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S");
}




Servo myServo;
const int SERVO_PIN = 13;


void setup(){
  Serial.begin(115200);
   pinMode(BOOT_PIN, INPUT_PULLUP);
  myServo.attach(SERVO_PIN);
  WiFi.mode(WIFI_STA);
  myServo.write(0);
  delay(1500);
   myServo.detach();
  WiFi.disconnect();
  delay(100);
  
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("connected!!!");
   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();


  dfSD.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1500);

  if (player.begin(dfSD)) 
  {
    Serial.println("OK");
    player.volume(30); 
  } 
  else 
  {
    Serial.println("Connecting to DFPlayer failed!");
  }


}

 


void loop()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  Serial.println(&timeinfo, "%H:%M:%S");
  if (timeinfo.tm_min == 0 && timeinfo.tm_hour != lastHour) {
    lastHour = timeinfo.tm_hour;
    everyhour();
  }

  if (digitalRead(BOOT_PIN) == LOW) {
    everyhour();
  }
  delay(1000);
}


void everyhour(){
      Serial.println("hour hits");
        Serial.println("Servo attached");
    myServo.attach(SERVO_PIN);
    Serial.println("moving servo out");
      myServo.write(180);
       player.play(1);
      delay(5000);
       Serial.println("moving servo in");
      myServo.write(0);
       delay(10000);
         myServo.detach();
       Serial.println("moving detached");

}



