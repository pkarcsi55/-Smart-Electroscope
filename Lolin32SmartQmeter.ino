// Smart Qmeter - ESP32-based Charge Sensor
// Developed by Pláth, 2025.07 | WeMos LOLIN32 Lite

#include <Wire.h>
#include "BluetoothSerial.h"

// Bluetooth sanity check
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

// --- Physical constants and ADC conversion ---
#define CAPACITANCE_HIGH_RANGE  0.560  // nF (560 pF)
#define CAPACITANCE_LOW_RANGE   4.700  // nF
#define VOLTAGE_DIVIDER_RATIO   0.325  // 3.9k / (8.1k + 3.9k)
#define ADC_RESOLUTION          4096.0 // 12-bit ADC
#define ADC_REF_VOLTAGE         3.3    // Volts

// Charge per ADC step in nanoCoulombs (nC)
#define NANO_C_PER_STEP_HIGH  ((CAPACITANCE_HIGH_RANGE * 1e-9 * ADC_REF_VOLTAGE) / (ADC_RESOLUTION * VOLTAGE_DIVIDER_RATIO) * 1e9)
#define NANO_C_PER_STEP_LOW   ((CAPACITANCE_LOW_RANGE  * 1e-9 * ADC_REF_VOLTAGE) / (ADC_RESOLUTION * VOLTAGE_DIVIDER_RATIO) * 1e9)

// --- LED PWM settings ---
const int pwmFreq = 500;
const int pwmRes = 8;

// --- Digital pins ---
const int autoZeroPin = 13;      // Controls auto-zero via DG444
const int rangeSwitchPin = 15;   // Selects range via DG444
const int ledGreen1 = 32;
const int ledGreen2 = 25;
const int ledRed1   = 26;
const int ledRed2   = 33;
const int analogInputPin = 27;

float nanoC = NANO_C_PER_STEP_HIGH;
float Qmax = CAPACITANCE_HIGH_RANGE * 8.0;  // For ±4 V swing
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
  // Configure PWM for LEDs
  ledcAttach(ledGreen1, pwmFreq, pwmRes); ledcWrite(ledGreen1, 255);
  ledcAttach(ledGreen2, pwmFreq, pwmRes); ledcWrite(ledGreen2, 255);
  ledcAttach(ledRed1,   pwmFreq, pwmRes); ledcWrite(ledRed1, 255);
  ledcAttach(ledRed2,   pwmFreq, pwmRes); ledcWrite(ledRed2, 255);

  // Set pin modes
  pinMode(autoZeroPin, OUTPUT);
  pinMode(rangeSwitchPin, OUTPUT);
  digitalWrite(rangeSwitchPin, HIGH); // Default: high sensitivity (520 pF)
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(autoZeroButton.pin, INPUT_PULLUP);
  attachInterrupt(autoZeroButton.pin, handleButtonPress, FALLING);

  // Serial and Bluetooth initialization
  Serial.begin(9600);
  SerialBT.begin("Qmeter"); // Bluetooth device name
  delay(200);

  // Startup LED sequence
  digitalWrite(LED_BUILTIN, LOW); // Indicate startup
  ledcWrite(ledGreen1, 0); ledcWrite(ledGreen2, 0);
  ledcWrite(ledRed1, 0);   ledcWrite(ledRed2, 0);
  delay(500);
  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1, 255);   ledcWrite(ledRed2, 255);

  performAutoZero();
}

void loop() {
  // Read commands from USB serial
  if (Serial.available() > 0) {
    inputCommand = Serial.readString();
    inputCommand.trim();
    decodeSerialCommand();
  }

  // Read commands from Bluetooth
  if (SerialBT.available() > 0) {
    inputCommand = SerialBT.readString();
    inputCommand.trim();
    decodeSerialCommand();
  }

  // Handle physical auto-zero button
  if (autoZeroButton.pressed) {
    performAutoZero();
    autoZeroButton.pressed = false;
    digitalWrite(LED_BUILTIN, HIGH); // Signal with built-in LED
  }

  // Measure average ADC value
  rawValue = 0;
  for (int i = 0; i < 15; i++) {
    rawValue += analogRead(analogInputPin);
    delay(20);
  }
  rawValue = rawValue / 15.0;

  // Calculate signed charge (Q)
  int delta = rawValue - zeroLevel;
  Q = -delta * nanoC;

  // Output to serial and Bluetooth
  Serial.println(String(signedFactor * Q, 2));
  SerialBT.println(String(signedFactor * Q, 2));

  updateChargeIndicatorLEDs();
  delay(20);
}

// --- Sets zeroLevel to current ADC baseline ---
void performAutoZero() {
  // Flash all LEDs briefly
  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1, 255);   ledcWrite(ledRed2, 255);

  // Trigger hardware zeroing via DG444
  digitalWrite(autoZeroPin, LOW);
  delay(500);
  digitalWrite(autoZeroPin, HIGH);

  // Average ADC reading
  int sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += analogRead(analogInputPin);
    delay(20);
  }
  zeroLevel = sum / 15.0;
  delay(200);
}

// --- Parses incoming single-character commands ---
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
    digitalWrite(rangeSwitchPin, HIGH); // Select 520 pF
    delay(100);
    performAutoZero();
  } else if (inputCommand == "L") {
    nanoC = NANO_C_PER_STEP_LOW;
    Qmax = CAPACITANCE_LOW_RANGE * 10.0;
    digitalWrite(rangeSwitchPin, LOW); // Select 4.7 nF
    delay(100);
    performAutoZero();
  }

  inputCommand = ""; // Clear buffer
}

// --- LED brightness visualization of measured charge ---
void updateChargeIndicatorLEDs() {
  float absQ = abs(Q);
  if (absQ < 0.1) absQ = 0;

  // Turn all LEDs off first (PWM 255 = off)
  ledcWrite(ledGreen1, 255); ledcWrite(ledGreen2, 255);
  ledcWrite(ledRed1,   255); ledcWrite(ledRed2,   255);

  if (Q > 0) {
    // Indicate positive charge using red LEDs
    if (absQ < Qmax / 2.0) {
      ledcWrite(ledRed1, 255 - 255 * absQ / (Qmax / 2.0));
    } else {
      ledcWrite(ledRed1, 0);
      absQ -= Qmax / 2.0;
      ledcWrite(ledRed2, 255 - 255 * absQ / (Qmax / 2.0));
    }
  } else {
    // Indicate negative charge using green LEDs
    if (absQ < Qmax / 2.0) {
      ledcWrite(ledGreen1, 255 - 255 * absQ / (Qmax / 2.0));
    } else {
      ledcWrite(ledGreen1, 0);
      absQ -= Qmax / 2.0;
      ledcWrite(ledGreen2, 255 - 255 * absQ / (Qmax / 2.0));
    }
  }
}
