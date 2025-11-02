# ⚡ SmartQmeter – Electrostatic Charge Indicator Device

## Description

This project implements a low-cost, real-time **electrometer-style charge sensor** using an ESP32 development board and an op-amp-based integrator circuit.  
The system is capable of detecting small electric charges by measuring the output voltage of the integrator and converting it into charge (in nanoCoulombs) based on a known capacitance.

---

## ⚙️ System Overview

- **Core MCU**: ESP32 (WeMos LOLIN32 Lite)
- **Analog frontend**: Inverting integrator powered by +12 V supply
- **Capacitor switching**: DG444 analog switch IC
- **Communication**: USB Serial and Bluetooth Serial (device name: `Qmeter`)
- **User interaction**: Physical auto-zero button + serial command interface
- **Visual feedback**: PWM-controlled RGB LEDs indicating charge level and polarity

---

## 🧮 Measurement Principle

- The integrator converts input charge to output voltage.
- A voltage divider (8.1 kΩ / 3.9 kΩ) scales the ±5 V swing (centered around +6 V) to fit the ESP32 ADC input range.
- The ESP32 samples this voltage and subtracts a baseline (`zeroLevel`) captured during auto-zeroing.
- The resulting difference is converted to charge using the known capacitance value and scaling constants.

---

## 🔁 Supported Commands

You can send single-character commands via USB or Bluetooth Serial:

| Command | Action                                |
|---------|---------------------------------------|
| `Z`     | Perform auto-zero                     |
| `H`     | Switch to high sensitivity (560 pF)   |
| `L`     | Switch to low sensitivity (4.7 nF)    |
| `+`     | Display positive polarity as-is       |
| `-`     | Invert displayed polarity             |

---

## 🌈 LED Indicators

- **Red LEDs**: Indicate **positive charge**
- **Green LEDs**: Indicate **negative charge**
- The **brightness** of each LED
