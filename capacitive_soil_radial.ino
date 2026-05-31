/*
capacitive soil v1.2 

Wire Connection:
1. Wiring the Sensor to Arduino
Sensor Pin	Connects To
VCC	3.3V or 5V (depends on sensor)
GND	GND
AOUT	A0 (Analog input)
⚠️ Most capacitive sensors work with 3.3V–5V. Check your sensor’s label or datasheet.
2. OLED wire connecction
Wiring: I2C OLED
OLED Pin	Arduino Uno / Mega
VCC	5V
GND	GND
SDA	A4 (Uno) / 20 (Mega)
SCL	A5 (Uno) / 21 (Mega)
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SOIL_SENSOR_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Chart settings
const int centerX = SCREEN_WIDTH / 2;
const int centerY = SCREEN_HEIGHT / 2 + 8;
const int radius = 20;
const int thickness = 3;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while (true);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  int rawValue = analogRead(SOIL_SENSOR_PIN);
  int moisturePercent = map(rawValue, 1023, 300, 0, 100);  // dry=1023, wet=300
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println(" %");

  display.clearDisplay();

  // === Draw Radial Progress Arc ===
  int angleStart = -90; // Start at top
  int angleEnd = map(moisturePercent, 0, 100, 0, 360);

  for (int r = radius - thickness; r <= radius; r++) {
    for (int angle = 0; angle <= angleEnd; angle++) {
      float rad = (angle + angleStart) * PI / 180.0;
      int x = centerX + r * cos(rad);
      int y = centerY + r * sin(rad);
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }

  // === Draw % Text ===
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(centerX - 12, centerY - 4);
  display.print(moisturePercent);
  display.print("%");

  // === Label ===
  display.setCursor(centerX - 25, centerY + radius + 6);
  display.print("Soil Moisture");

  display.display();
  delay(1000);
}


