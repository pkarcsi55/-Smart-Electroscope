# ⚡ Q_meter – Real-time Smart Electroscope Visualizer

**Q_meter** is an Android application built with [MIT App Inventor](https://appinventor.mit.edu/).  
It serves as a **real-time visual interface** for the **Smart Electroscope**, an ESP32-based electronic instrument that detects and quantifies electrostatic charge.

---

## 🔗 Bluetooth Connection

- The user connects to a previously paired Bluetooth device whose name contains *Qmeter* or *Electroscope*.
- Once connected, the app receives data from the electroscope every **20 ms**.
- Connection status is indicated both **visually** (red/green button) and **audibly** (using Text-to-Speech feedback).

---

## 📊 Data Reception & Visualization

- The app displays the measured charge as a **colored circle**:
  - 🔴 **Red** – positive charge  
  - 🟢 **Green** – negative charge  
  - ⚪ **Gray** – neutral or near-zero charge
- The circle radius reflects the intensity of the detected electric field, providing a visual measure of charge influence rather than charge quantity.  
- The **numerical charge value** (e.g., `+3.2 nC`) is shown below the circle, with the **sign** (`+` / `–`) highlighted separately.

---

## 🛠️ Technical Components (MIT App Inventor Blocks)

- **BluetoothClient1** – manages serial communication  
- **Canvas1** – renders the dynamic charge indicator  
- **Timer1** – polls sensor data every 20 ms  
- **TextToSpeech1** – provides voice feedback  
- **Notifier1** – handles dialogs and error messages

---

## ⚙️ Additional Features

- Before disconnecting, the app sends the command `"X\n"` to stop sensor transmission.  
- Handles missing Bluetooth adapter, unpaired devices, and connection errors gracefully.  
- Uses a **dark, high-contrast UI** for classroom visibility and minimal distraction.

---

## 🎓 Purpose and Use

The **Q_meter** app is designed for **educational demonstrations** in physics classes and labs.  
It enables students to **visualize electrostatic phenomena in real time**, turning abstract charge concepts into **interactive visual experiences**.

---

📦 **Source and Reuse**

The project source file (`Qmeter.aia`) is included in this repository and can be imported directly into [MIT App Inventor](https://appinventor.mit.edu/) for modification or extension.

> Repository: [Smart-Electroscope on GitHub](https://github.com/pkarcsi55/Smart-Electroscope)

