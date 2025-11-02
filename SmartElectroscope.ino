// Smart Qmeter - ESP32-based Charge Sensor (Power-Optimized Edition)
// Developed by Piláth, 2025.11 | WeMos LOLIN32 Lite

#include <Wire.h>
#include "BluetoothSerial.h"
#include <WiFi.h>

// --- Bluetooth sanity check ---
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

// --- Physical constants and ADC conversion ---
#define CAPACITANCE_HIGH_RANGE  0.560  // nF (560 pF)
#define CAPACITANCE_LOW_RANGE   4.700  // nF
#define VOLTAGE_DIVIDER_RATIO   0.3    // 3.9k / (8.1k + 3.9k 0.325)
#define ADC_RESOLUTION          4096.0
#define ADC_REF_VOLTAGE         3.3

#define NANO_C_PER_STEP_HIGH  ((CAPACITANCE_HIGH_RANGE * 1e-9 * ADC_REF_VOLTAGE) / (ADC_RESOLUTION * VOLTAGE_DIVIDER_RATIO) * 1e9)
#define NANO_C_PER_STEP_LOW   ((CAPACITANCE_LOW_RANGE  * 1e-9 * ADC_REF_VOLTAGE) / (ADC_RESOLUTION * VOLTAGE_DIVIDER_RATIO) * 1e9)

// --- LED PWM settings ---
const int pwmFreq = 500;
const int pwmRes = 8;

// --- Digital pins ---
const int autoZeroPin = 13;
const int rangeSwitchPin = 15;
const int ledGreen1 = 32;
const int ledGreen2 = 25;
const int ledRed1   = 26;
const int ledRed2   = 33;
const int analogInputPin = 27;

float nanoC = NANO_C_PER_STEP_HIGH;
float Qmax = CAPACITANCE_HIGH_RANGE * 6;
float zeroLevel = 0.0;
float Q = 0;
int rawValue = 0;
int signedFactor = 1;
String inputCommand = "";

// --- Button configuration ---
struct Button {
  const uint8_t pin;
  uint32_t pressCount;
  bool pressed;
};

Button autoZeroButton = {4, 0, false};

// Interrupt Service Routine for button
void IRAM_ATTR handleButtonPress() {
  autoZeroButton.pressCount++;
  autoZeroButton.pressed = true;
}

void setup() {
  // Disable Wi-Fi to save power
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect(true);
  btStart();   // make sure BT is active
  delay(100);

  // Configure PWM for LEDs
  ledcAttach(ledGreen1, pwmFreq, pwmRes); ledcWrite(ledGreen1, 255);
  ledcAttach(ledGreen2, pwmFreq, pwmRes); ledcWrite(ledGreen2, 255);
  ledcAttach(ledRed1,   pwmFreq, pwmRes); ledcWrite(ledRed1, 255);
  ledcAttach(ledRed2,   pwmFreq, pwmRes); ledcWrite(ledRed2, 255);

  pinMode(autoZeroPin, OUTPUT);
  pinMode(rangeSwitchPin, OUTPUT);
  digitalWrite(rangeSwitchPin, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(autoZeroButton.pin, INPUT_PULLUP);
  attachInterrupt(autoZeroButton.pin, handleButtonPress, FALLING);

  Serial.begin(9600);
  SerialBT.begin("Qmeter");
  delay(200);

  // Startup LED animation
  digitalWrite(LED_BUILTIN, LOW);
  ledcWrite(ledGreen1, 0); ledcWrite(ledGreen2, 0);
  ledcWrite(ledRed1, 0);   ledcWrite(ledRed2, 0);
  delay(300);
  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1, 255);   ledcWrite(ledRed2, 255);

  performAutoZero();
}

void loop() {
  // USB serial commands
  if (Serial.available() > 0) {
    inputCommand = Serial.readString();
    inputCommand.trim();
    decodeSerialCommand();
  }

  // Bluetooth commands
  if (SerialBT.available() > 0) {
    inputCommand = SerialBT.readString();
    inputCommand.trim();
    decodeSerialCommand();
  }

  // Button handler
  if (autoZeroButton.pressed) {
    performAutoZero();
    autoZeroButton.pressed = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // --- Measurement ---
  rawValue = 0;
  for (int i = 0; i < 15; i++) {
    rawValue += analogRead(analogInputPin);
    delay(20);
  }
  rawValue = rawValue / 15.0;

  int delta = rawValue - zeroLevel;
  Q = -delta * nanoC;

  Serial.println(String(signedFactor * Q, 2));
  SerialBT.println(String(signedFactor * Q, 2));

  updateChargeIndicatorLEDs();

  // Allow BT stack and CPU to idle a bit → lowers temperature
  delay(10);
}

// --- Auto-zero ---
void performAutoZero() {
  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1, 255);   ledcWrite(ledRed2, 255);

  digitalWrite(autoZeroPin, LOW);
  delay(500);
  digitalWrite(autoZeroPin, HIGH);

  int sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += analogRead(analogInputPin);
    delay(20);
  }
  zeroLevel = sum / 15.0;
  delay(200);
}

// --- Serial/Bluetooth command handler ---
void decodeSerialCommand() {
  Serial.println(inputCommand);

  if (inputCommand == "-") {
    signedFactor = -1;
  } else if (inputCommand == "+") {
    signedFactor = 1;
  } else if (inputCommand == "Z") {
    performAutoZero();
  } else if (inputCommand == "H") {
    nanoC = NANO_C_PER_STEP_HIGH;
    Qmax = CAPACITANCE_HIGH_RANGE * 10.0;
    digitalWrite(rangeSwitchPin, HIGH);
    delay(100);
    performAutoZero();
  } else if (inputCommand == "L") {
    nanoC = NANO_C_PER_STEP_LOW;
    Qmax = CAPACITANCE_LOW_RANGE * 10.0;
    digitalWrite(rangeSwitchPin, LOW);
    delay(100);
    performAutoZero();
  }

  inputCommand = "";
}

// --- LED charge indicator ---
void updateChargeIndicatorLEDs() {
  float absQ = abs(Q);
  if (absQ < 0.1) absQ = 0;

  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1,   255); ledcWrite(ledRed2,   255);

  if (Q > 0) {
    if (absQ < Qmax / 2.0) {
      ledcWrite(ledRed1, 255 - 255 * absQ / (Qmax / 2.0));
    } else {
      ledcWrite(ledRed1, 0);
      absQ -= Qmax / 2.0;
      ledcWrite(ledRed2, 255 - 255 * absQ / (Qmax / 2.0));
    }
  } else {
    if (absQ < Qmax / 2.0) {
      ledcWrite(ledGreen1, 255 - 255 * absQ / (Qmax / 2.0));
    } else {
      ledcWrite(ledGreen1, 0);
      absQ -= Qmax / 2.0;
      ledcWrite(ledGreen2, 255 - 255 * absQ / (Qmax / 2.0));
    }
  }
}
