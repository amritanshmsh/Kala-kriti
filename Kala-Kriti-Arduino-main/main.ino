#include <Adafruit_LiquidCrystal.h> 

Adafruit_LiquidCrystal lcd(0);
int sensor = A0;
int buzzer = 2;
int red = 10;
int green = 8;
int blue = 9;
int gasLevel = 0; 
String quality = "";

void setup() {
  
  Serial.begin(9600); 
  pinMode(buzzer, OUTPUT); 
  pinMode(sensor, INPUT); 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  lcd.begin(16, 2); 
  lcd.clear(); 
  lcd.setBacklight(1); 
  lcd.setCursor(0, 1);
  lcd.print("KALA-KRITI");
  delay(2000);
}

void loop() {
  
  gasLevel = analogRead(sensor);
  Serial.print(" Gas Level:");
  Serial.println(gasLevel);
  digitalWrite (green, LOW);
  digitalWrite (red, LOW);
  digitalWrite (blue, LOW);
  digitalWrite (buzzer, LOW);

  if (gasLevel <= 320) {
    quality = "GOOD";
    digitalWrite (green, HIGH);
    delay(100);
  }

  else if (gasLevel > 320 and gasLevel < 700) {
    quality = "Unhealthy";
    digitalWrite (blue, HIGH);
    tone(buzzer, 500, 50);
    delay(100);
  }

  else if (gasLevel > 700) {
    quality = "Hazardous ";
    digitalWrite (red, HIGH);
    tone( buzzer, 800, 1000);
    delay(100);
  }

  lcd.clear();
  lcd.setBacklight(1);
  lcd.setCursor(0, 0);
  lcd.print("Air Quality is");
  lcd.setCursor(0, 1);
  lcd.print(quality);
  delay(500);
} 
