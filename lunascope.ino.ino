#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Servo.h>

// =====================================================
// OLED DISPLAY - PAGE BUFFER VERSION
// Saves Arduino UNO RAM
// =====================================================
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(
  U8G2_R0,
  U8X8_PIN_NONE
);

// =====================================================
// DHT22
// =====================================================
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =====================================================
// HC-SR04
// =====================================================
#define TRIG_PIN 8
#define ECHO_PIN 9

// =====================================================
// ANALOG SENSORS
// =====================================================
#define MQ135_PIN A0
#define RAIN_PIN A1
#define LDR_PIN A2

// =====================================================
// LED
// =====================================================
#define LED_PIN 6

// =====================================================
// SERVOS
// =====================================================
#define SERVO_HORIZONTAL_PIN 5
#define SERVO_VERTICAL_PIN 7

Servo servoHorizontal;
Servo servoVertical;

// =====================================================
// VARIABLES
// =====================================================
float temperature;
float humidity;

int distanceCenter;
int distanceLeft;
int distanceRight;

int mq135Value;
int rainValue;
int ldrValue;

// =====================================================
// OBSTACLE DISTANCE
// =====================================================
#define OBSTACLE_DISTANCE 20

// =====================================================
// GET ULTRASONIC DISTANCE
// =====================================================
int getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  int distance = duration * 0.0343 / 2;

  return distance;
}

// =====================================================
// SCAN LEFT / CENTER / RIGHT
// =====================================================
void scanObstacle() {

  // CENTER
  servoHorizontal.write(90);
  delay(300);
  distanceCenter = getDistance();

  // LEFT
  servoHorizontal.write(30);
  delay(300);
  distanceLeft = getDistance();

  // RIGHT
  servoHorizontal.write(150);
  delay(300);
  distanceRight = getDistance();

  // Return CENTER
  servoHorizontal.write(90);
}

// =====================================================
// DISPLAY PAGE 1
// =====================================================
void showEnvironmentData() {

  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);

    display.drawStr(0, 10, "ENVIRONMENT MONITOR");

    display.drawHLine(0, 13, 128);

    // Temperature
    display.setCursor(0, 27);
    display.print("Temp: ");

    if (isnan(temperature)) {
      display.print("ERROR");
    }
    else {
      display.print(temperature, 1);
      display.print(" C");
    }

    // Humidity
    display.setCursor(0, 39);
    display.print("Humidity: ");

    if (isnan(humidity)) {
      display.print("ERROR");
    }
    else {
      display.print(humidity, 1);
      display.print("%");
    }

    // Air quality sensor
    display.setCursor(0, 51);
    display.print("Air: ");
    display.print(mq135Value);

    // Rain
    display.setCursor(0, 63);
    display.print("Rain: ");
    display.print(rainValue);

  } while (display.nextPage());
}

// =====================================================
// DISPLAY PAGE 2
// =====================================================
void showRobotData() {

  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);

    display.drawStr(0, 10, "SMART MONITOR");

    display.drawHLine(0, 13, 128);

    // Light
    display.setCursor(0, 26);
    display.print("Light: ");
    display.print(ldrValue);

    // Center
    display.setCursor(0, 38);
    display.print("Center: ");
    display.print(distanceCenter);
    display.print("cm");

    // Left
    display.setCursor(0, 50);
    display.print("Left: ");
    display.print(distanceLeft);
    display.print("cm");

    // Right
    display.setCursor(0, 62);
    display.print("Right: ");
    display.print(distanceRight);
    display.print("cm");

  } while (display.nextPage());
}

// =====================================================
// SERIAL MONITOR
// =====================================================
void printData() {

  Serial.println();
  Serial.println("----------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("MQ135: ");
  Serial.println(mq135Value);

  Serial.print("Rain: ");
  Serial.println(rainValue);

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  Serial.println();

  Serial.print("Left: ");
  Serial.print(distanceLeft);
  Serial.println(" cm");

  Serial.print("Center: ");
  Serial.print(distanceCenter);
  Serial.println(" cm");

  Serial.print("Right: ");
  Serial.print(distanceRight);
  Serial.println(" cm");

  if (distanceCenter < OBSTACLE_DISTANCE) {
    Serial.println("OBSTACLE DETECTED!");
  }
  else {
    Serial.println("PATH CLEAR");
  }

  Serial.println("----------------------------");
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  Serial.begin(9600);

  // DHT
  dht.begin();

  // HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LED
  pinMode(LED_PIN, OUTPUT);

  // Servos
  servoHorizontal.attach(SERVO_HORIZONTAL_PIN);
  servoVertical.attach(SERVO_VERTICAL_PIN);

  // Center servos
  servoHorizontal.write(90);
  servoVertical.write(90);

  // Display
  display.begin();

  // Startup screen
  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);

    display.drawStr(20, 25, "SMART");
    display.drawStr(5, 40, "ENVIRONMENT");
    display.drawStr(25, 55, "SYSTEM");

  } while (display.nextPage());

  delay(2000);

  Serial.println("================================");
  Serial.println("SMART ENVIRONMENT SYSTEM");
  Serial.println("SYSTEM STARTED");
  Serial.println("================================");
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop() {

  // ===================================================
  // DHT22
  // ===================================================

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // ===================================================
  // ANALOG SENSORS
  // ===================================================

  mq135Value = analogRead(MQ135_PIN);

  rainValue = analogRead(RAIN_PIN);

  ldrValue = analogRead(LDR_PIN);

  // ===================================================
  // LDR → LED
  // ===================================================

  if (ldrValue < 400) {

    digitalWrite(LED_PIN, HIGH);

  }
  else {

    digitalWrite(LED_PIN, LOW);

  }

  // ===================================================
  // ULTRASONIC SCAN
  // ===================================================

  scanObstacle();

  // ===================================================
  // SERIAL OUTPUT
  // ===================================================

  printData();

  // ===================================================
  // DISPLAY PAGE 1
  // ===================================================

  showEnvironmentData();

  delay(2500);

  // ===================================================
  // DISPLAY PAGE 2
  // ===================================================

  showRobotData();

  delay(2500);
}