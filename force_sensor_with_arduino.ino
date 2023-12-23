#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET    -1
Adafruit_SSD1306 display(OLED_RESET);

const int forceSensorPin = A0; 
const int buzzerPin = 2;
const int greenLEDPin = 3;
const int yellowLEDPin = 4;
const int redLEDPin = 5;

int sensorValue = 0;

// Thresholds for sensor values
const int lowThreshold = 200;  // Example threshold for low force
const int mediumThreshold = 500; // Example threshold for medium force
const int highThreshold = 550;  // Example threshold for high force

void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop() {
  sensorValue = analogRead(forceSensorPin);

  // Display the reading on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Force: ");
  display.println(sensorValue);

  // Draw a simple graph
  int graphHeight = map(sensorValue, 0, 1023, 0, display.height());
  display.fillRect(50, display.height() - graphHeight, 10, graphHeight, WHITE);
  display.display();

  // Control LEDs based on sensor value
  if (sensorValue < lowThreshold) {
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
  } else if (sensorValue < mediumThreshold) {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
  } else {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
  }

  // Activate buzzer if sensor value exceeds high threshold
  if (sensorValue > highThreshold) {
    tone(buzzerPin, 1000); // 1000 Hz tone
  } else {
    noTone(buzzerPin);
  }

  delay(200); // Update interval
}
