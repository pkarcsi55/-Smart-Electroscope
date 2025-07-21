🔧 Qmeter ESP32 – Charge Measurement Device
Description:
This project implements a low-cost, real-time electrometer-style charge sensor using an ESP32 development board and an op-amp-based integrator circuit. The system is capable of detecting small electric charges by measuring the output voltage of the integrator and converting it into charge (in nanoCoulombs) based on a known capacitance.

⚙️ System Overview
Core MCU: ESP32 (WeMos LOLIN32 Lite)

Analog frontend: Inverting integrator powered by +12 V supply

Capacitor switching: DG444 analog switch IC

Communication: USB Serial and Bluetooth Serial (device name: Qmeter)

User interaction: Physical auto-zero button + command interface

Visual feedback: RGB LEDs indicating charge level and polarity

🧮 Measurement Principle
The integrator converts input charge to voltage.

A voltage divider (8.1 kΩ / 3.9 kΩ) scales the ±5 V swing (around +6 V) to fit the ESP32 ADC range.

The ESP32 samples this voltage and subtracts a baseline (zeroLevel) captured during auto-zeroing.

The resulting difference is converted to charge using the known capacitance value and scaling constants.

🔁 Supported Commands
These can be sent over USB serial or Bluetooth:

Command	Action
Z	Perform auto-zero
H	Switch to high sensitivity (520 pF)
L	Switch to low sensitivity (4.7 nF)
+	Display positive polarity as-is
-	Invert displayed polarity

🌈 LED Indicators
Red LEDs: Positive charge

Green LEDs: Negative charge

The brightness of each LED indicates the magnitude of the charge.

The system uses PWM to create smooth brightness transitions.

📐 Calibration and Accuracy
All constants (nanoC per ADC step) are calculated from:

Capacitance values (520 pF and 4.7 nF)

Voltage divider ratio (0.325)

ADC resolution (12-bit)

ESP32 reference voltage (3.3 V)

The system assumes a ±5 V swing around +6 V from the integrator, covering roughly 0.325–3.575 V on the ADC input.

📤 Output
The measured charge is printed continuously to:

USB Serial (baud: 9600)

Bluetooth Serial (as plain text)

Format: Signed charge in nanoCoulombs with two decimal places
