// Scl of OLED or I2C is connected to D1 or GPIO 5 & SDA of OLED or I2C is connected to D2 or GPIO 4

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>        // library for I2C LCD  
LiquidCrystal_I2C lcd(0x27, 16, 2);   // set the LCD address to 0x27 for a 16 chars and 2 line display

int sensor = A0;
int relay = 13;                                //13 is GPIO pin  in NodeMcu & D7 is digital pin in Node Mcu or 0 is GPIO pin  in NodeMcu & D3 is digital pin in Node Mcu
int buzzer = 16;                              //16 is GPIO pin  in NodeMcu & D0 is digital pin in Node Mcu
int red = 2;                                 //2 is GPIO pin  in NodeMcu & D4 is digital pin in Node Mcu
int green = 14;                             //14 is GPIO pin  in NodeMcu & D5 is digital pin in Node Mcu
int blue = 12;                            //12 is GPIO pin  in NodeMcu & D6 is digital pin in Node Mcu
int gasLevel = 0;                        //int variable for gas level
String quality = "";

String host = "api.thingspeak.com";
int httpPort = 80;
String url1 = "/update?api_key=FPDASLMRFLSARQ97&field1=";
String url2 = "/update?api_key=FPDASLMRFLSARQ97&field2=";
HTTPClient http;
WiFiClient wifiClient;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();    // initialize the lcd
  lcd.backlight(); // backlight ON
  lcd.print("KALA-KRITI");
  pinMode(buzzer, OUTPUT);                       //set beeper for output
  pinMode(sensor, INPUT);                       //set sensor for input
  pinMode (relay, OUTPUT);
  //pinMode (fan, OUTPUT);
  pinMode (red, OUTPUT);
  pinMode (green, OUTPUT);
  pinMode (blue, OUTPUT);

  WiFi.begin("wifi_name", "wifi_pass");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());

}

void loop()
{

  gasLevel = analogRead(sensor);
  Serial.println("Mq2 Value:" + (String) gasLevel);
  digitalWrite (green, LOW);
  digitalWrite (red, LOW);
  digitalWrite (blue, LOW);
  digitalWrite (buzzer, LOW);
  //digitalWrite (fan, LOW);
  digitalWrite (relay, HIGH);
  String link1  = url1 + (String) gasLevel;
  http.begin(wifiClient, 
  host, httpPort, link1);
  int httpCode = http.GET();
  Serial.println(httpCode);
  delay(2000);


  if (gasLevel < 400) {
    quality = "GOOD";
    int quality1 = 1;
    Serial.println("Air Quality:" + (String) quality);
    digitalWrite(relay, HIGH);
    digitalWrite (green, HIGH);
    String link2  = url2 + (String) quality1;
    http.begin(wifiClient, host, httpPort, link2);
    int httpCode = http.GET();
    Serial.println(httpCode);
    delay(2000);
  }

  else if (gasLevel >= 400 and gasLevel < 500) {
    quality = "Unhealthy";
   int quality2 = 2;
    Serial.println("Air Quality:" + (String) quality);
    digitalWrite(relay, LOW);
    digitalWrite (blue, HIGH);
    tone(buzzer, 1000, 80);
    String link3  = url2 + (String) quality2;
    http.begin(wifiClient, host, httpPort, link3);
    int httpCode = http.GET();
    Serial.println(httpCode);
    delay(2000);
  }

  else if (gasLevel >= 500) {
    quality = "Hazardous ";
   int quality3 = 3;
    Serial.println("Air Quality:" + (String) quality);
    digitalWrite(relay, LOW);
    //digitalWrite(fan, HIGH);
    digitalWrite (red, HIGH);
    digitalWrite( buzzer, HIGH);
    String link4  = url2 + (String) quality3;
    http.begin(wifiClient, host, httpPort, link4);
    int httpCode = http.GET();
    Serial.println(httpCode);
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality");
  lcd.setCursor(12, 0);
  lcd.print(gasLevel);
  lcd.setCursor(1, 1);
  lcd.println(quality);

}
