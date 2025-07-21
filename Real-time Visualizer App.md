# Q_meter – Real-time Electrostatic Charge Visualizer

**Q_meter** is an Android application built with [MIT App Inventor](https://appinventor.mit.edu/) that acts as a real-time visual interface for a Bluetooth-enabled **electrostatic charge measuring device** (e.g., an ESP32-based Qmeter).

## 🔗 Bluetooth Connection

- The user can connect to a previously paired device whose name contains *Qmeter*.
- Upon successful connection, the app reads incoming data every 20 ms via Bluetooth.
- Connection status is indicated both visually (red/green button) and audibly (using TextToSpeech).

## 📊 Data Reception & Visualization

- The app processes the measured charge value (in nC) and displays it as a **colored circle**, where:
  - 🔴 Red: positive charge
  - 🟢 Green: negative charge
  - ⚪ Gray: near-zero charge
- The radius of the circle is proportional to the absolute charge value.
- The numerical charge value (e.g., `+3.2 nC`) is also displayed.
- The sign of the charge is shown separately as a `+`, `–`, or empty if close to zero.

## 🛠️ Technical Details

- `BluetoothClient1`: handles Bluetooth communication
- `Canvas1`: renders the colored circle
- `Timer1`: polls data every 20 ms
- `TextToSpeech1`: provides spoken feedback
- `Notifier1`: handles dialogs and error messages

## ⚙️ Additional Features

- Before disconnecting, the app sends an `"X\n"` command to signal the sensor to stop.
- Handles various edge cases: missing Bluetooth, no paired devices, failed connection, etc.
- The user interface uses a dark theme with high-contrast colors and minimal distractions.

## 📦 Purpose

This application is designed primarily for **educational use** in classrooms and labs. It allows students to visualize electrostatic charge behavior in real time, bridging abstract concepts with interactive feedback.

---

> The project source is available in `.aia` format in this repository (https://github.com/pkarcsi55/-Smart-Qmeter/blob/main/Qmeter.aia). Suitable for modification and extension in MIT App Inventor.
