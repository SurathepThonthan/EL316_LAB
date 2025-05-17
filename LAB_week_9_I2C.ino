#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ledPin = 13; // Built-in LED on Arduino Uno

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if needed

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Sliding "Arduino Uno" message
  String message = "   Arduino Uno   "; // Pad with spaces for smooth scroll
  lcd.setCursor(0, 0);
  lcd.print(message.substring(0, 16));
  delay(500);

  // Scroll the message left
  for (int i = 0; i < message.length() - 16; i++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }

  Serial.println("I2C Scanner starting...");
}

void loop() {
  byte error, address;
  int count = 0;

  Serial.println("Scanning for I2C devices...");

  lcd.setCursor(0, 1); // Second line
  lcd.print("Scanning...    ");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");

      // Blink LED 3 times
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(150);
        digitalWrite(ledPin, LOW);
        delay(150);
      }

      count++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  Serial.print("Total I2C devices found: ");
  Serial.println(count);
  Serial.println("Scan complete.\n");

  // Update LCD with device count
  lcd.setCursor(0, 1);
  lcd.print("Found: ");
  lcd.print(count);
  lcd.print("       "); // Clear residual text

  delay(5000);
}
