/*
 * Project parti-badge-thermal-sensor
 * Description: Simple temperature gauge app for the gen3 Particle Spectra badge
 * Author: James Hagerman
 * Date: 2020-08-16
 */
  
#include "Adafruit_Si7021.h"
#include "Adafruit_SSD1306.h"

const unsigned char tempImage[] = {
  0B00000011,0B11000000
  ,0B00001111,0B11110000
  ,0B00011110,0B01111000
  ,0B00011000,0B00011000
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111000,0B00011100
  ,0B00111111,0B11111100
  ,0B00111111,0B11111100
  ,0B00111111,0B11111100
  ,0B00111111,0B11111100
  ,0B01111111,0B11111110
  ,0B01111111,0B11111110
  ,0B11111111,0B11111111
  ,0B11111111,0B11111111
  ,0B11111111,0B11111111
  ,0B11111111,0B11111111
  ,0B11111111,0B11111111
  ,0B11111111,0B11111111
  ,0B01111111,0B11111110
  ,0B01111111,0B11111110
  ,0B00111111,0B11111100
  ,0B00011111,0B11111000
  ,0B00000111,0B11100000
};

const unsigned char humidityImage[] = {
  0B00000000,0B01100000,0B00000000
  ,0B00000000,0B01100000,0B00000000
  ,0B00000000,0B11110000,0B00000000
  ,0B00000001,0B11111000,0B00000000
  ,0B00000011,0B11111100,0B00000000
  ,0B00000011,0B11111100,0B00000000
  ,0B00000111,0B11111110,0B00000000
  ,0B00001111,0B11111111,0B00000000
  ,0B00011111,0B11111111,0B10000000
  ,0B00011111,0B11111111,0B10000000
  ,0B00111100,0B01111111,0B11000000
  ,0B00111000,0B00111111,0B11000000
  ,0B01111001,0B00111001,0B11100000
  ,0B01111000,0B00110001,0B11100000
  ,0B11111100,0B01100011,0B11110000
  ,0B11111111,0B11000111,0B11110000
  ,0B11111111,0B10001111,0B11110000
  ,0B11111111,0B00011111,0B11110000
  ,0B11111110,0B00111111,0B11110000
  ,0B11111100,0B01100011,0B11110000
  ,0B01111000,0B11001001,0B11100000
  ,0B01111001,0B11001001,0B11100000
  ,0B00111111,0B11000001,0B11000000
  ,0B00111111,0B11110111,0B11000000
  ,0B00011111,0B11111111,0B10000000
  ,0B00000111,0B11111110,0B00000000
  ,0B00000001,0B11111000,0B00000000
};

// Initialize Si7021 sensor
Adafruit_Si7021 envSensor = Adafruit_Si7021();
float currentTemp;
float currentHumidity;
int currentTempRounded;
int currentHumidityRounded;

// Init Display
Adafruit_SSD1306 display(RESET);

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;

/* General Macros */
#define TEMP_CHECK_INTERVAL 1000

// Get temp and humidity from the sensors
void getTempAndHumidity() {
  int prevRoundedTemp = currentTempRounded;
  int prevRoundedHumidity = currentHumidityRounded;

  currentTemp = envSensor.readTemperature() * 1.8 + 32.00;
  currentHumidity = envSensor.readHumidity();

  currentTempRounded = round((currentTemp) * 10) / 10;
  currentHumidityRounded = round(currentHumidity * 10) / 10;

  // If either has changed and these values are being displayed, update the display
  if (prevRoundedTemp != currentTempRounded) {
    showTempAndHumidity();
    String tempData = String(currentTemp, 2);
    // Particle.publish("temperature-data", tempData, PRIVATE);

    String change = "no change";
    if (currentTempRounded > prevRoundedTemp) {
      change = String(currentTempRounded-prevRoundedTemp);
      // Particle.publish("temp-up", change, PRIVATE);
      Particle.publish("temp-up", tempData, PRIVATE);
    } else if (currentTempRounded < prevRoundedTemp) {
      change = String(prevRoundedTemp-currentTempRounded);
      // Particle.publish("temp-down", change, PRIVATE);
      Particle.publish("temp-down", tempData, PRIVATE);
    }
  }

  // showTempAndHumidity();
}

// Clear the OLED display
void clearScreen() {
  display.stopscroll();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.setTextWrap(true);
}

// Show the temperature and humidity on the display
void showTempAndHumidity() {
  clearScreen();

  display.drawBitmap(7, 18, tempImage, 16, 43, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(52, 16);
  display.println("Temp");
  display.setTextSize(2);

  // Decimal
  display.setCursor(35, 25);
  display.print(currentTemp, 2);

  // Rounded 
  // display.setCursor(48, 25);
  // display.print((int)currentTempRounded);

  display.println("f");
  display.setTextSize(1);
  display.setCursor(42, 42);
  display.println("Humidity");

  display.setTextSize(2);

  // Decimal
  display.setCursor(35, 50);
  display.print(currentHumidity, 2);

  // Rounded
  // display.setCursor(48, 50);
  // display.print((int)currentHumidityRounded);

  display.println("%");


  display.drawBitmap(105, 28, humidityImage, 20, 27, 1);
  display.display();
  display.setTextSize(1);
}

// Check the temp and humidity when called from a cloud function
int checkTempHandler(String data) {
  getTempAndHumidity();
  return 1;
}

void setup() {
  Particle.variable("currentTemp", currentTempRounded);
  Particle.variable("currentHu", currentHumidityRounded);
  Particle.function("checkTemp", checkTempHandler);

  Serial.begin();

  // Initialize Temp and Humidity sensor
  while (!envSensor.begin())
    ;

  // Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousEnvReading > TEMP_CHECK_INTERVAL) {
    previousEnvReading = currentMillis;
    getTempAndHumidity();
  }


}
