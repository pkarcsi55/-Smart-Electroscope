# ⚡ Smart Electroscope  
*Demonstrating Electrostatic Experiments with a Smart Electroscope*

The **Smart Electroscope** is a modern, Bluetooth-enabled electronic electroscope designed to visualize and quantify electrostatic phenomena in real time.  
It connects to an Android application created with MIT App Inventor, displaying charge polarity and magnitude through dynamic color and size indicators.

---

## ⚡ Charge Induction

This experiment demonstrates how friction between objects can result in charge transfer.  
A PVC rod is rubbed with a piece of wool fabric and then brought near the **electroscope’s sensing plate**.  
The mobile display shows a **green circle** (negative charge) instead of the neutral gray one.  
The **radius** of the circle — representing the detected charge magnitude — depends on rubbing intensity, duration, and the distance between the rod and the sensor.

The circle size decreases **non-linearly with distance**, highlighting the spatial dependence of the electric field.  
The wool cloth becomes **positively charged**; a **sponge** retains its charge longer due to its smoother surface, reducing discharge through airborne ions.

When the charged rod or cloth is removed, the indicator returns to **gray** (zero net charge).  
A glass rod rubbed with leather becomes **positively charged**, while the leather becomes **negatively charged**.

**Physical background:**  
The external charge **polarizes the electroscope’s plate**, inducing opposite charges.  
The sign of the induced charge depends on that of the external one.

---

## 🔄 Charge Transfer

If a pre-charged conductor (e.g., a metal sphere) on an insulating base touches the **electroscope’s plate**,  
the reading remains non-zero after removal — showing that part of the charge has transferred to the measuring capacitor.

The effect can also be demonstrated **without contact**.  
An aluminum soda can placed near the sensor acquires **induced charge** from a nearby negatively charged PVC rod.  
Because the can has higher **capacitance**, it can store **more charge**, even cumulatively by repeating the process.

---

## ⚖️ Electrostatic Charging by Separation

A negatively charged PVC rod is brought near a neutral aluminum can without touching it.  
The **Smart Electroscope** detects the redistribution of charge (green circle).  
Touching the far side of the can with a finger allows electrons to flow to ground.  
After lifting the finger and removing the rod, the can remains **positively charged**, indicated by a **red circle** on the display.

---

## ☀️ Demonstrating the Photoelectric Effect

This experiment highlights the sensitivity of the **Smart Electroscope**.  
A polished aluminum soda-can base is negatively charged and illuminated with **254 nm UV light**.  
The can gradually loses its negative charge — clearly visible on the display.

**Physical background:**  
254 nm photons have ~4.89 eV energy, exceeding the **work function of aluminum (4.28 eV)**,  
so they can **eject electrons**, reducing the negative charge.  
A clean, oxide-free surface is required, as Al₂O₃ blocks emission.

**Note:**  
If the can is initially **positively charged**, UV illumination produces **no effect**, confirming that only **electrons** are emitted.

---

## ⏱️ Time-Dependent Discharge Measurement

This test investigates how the negative charge of an aluminum can decays over time under UV light exposure.  
Values measured by the **Smart Electroscope** can be plotted in real time using  
[**SerialPlot v0.13**](https://github.com/hyOzd/serialplot), which displays COM-port data versus time.

> ⚠️ **Tip:** Turn on the UV source *before* starting the measurement —  
> sudden intensity changes can distort the initial data.

---

© 2025 Piláth Károly — Smart Electroscope Project  
[GitHub Repository](https://github.com/pkarcsi55/Smart-Electroscope)
