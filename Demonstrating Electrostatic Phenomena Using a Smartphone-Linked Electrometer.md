# Demonstrating Electrostatic Experiments with the Charge Measurement Device

## Charge Induction

This experiment demonstrates how friction between objects can result in charge transfer. A PVC rod is rubbed with a piece of wool fabric and then brought near the charge sensor. The display of the mobile device shows a green circle instead of the neutral gray one, indicating the presence of negative charge. The radius of the circle — representing the magnitude of the detected charge — depends on the intensity and duration of rubbing, as well as the distance between the rod and the sensor.

It can be observed that the circle size decreases in a nonlinear fashion with distance, highlighting the spatial behavior of the electric field.

The wool cloth used to rub the rod becomes positively charged. Based on experience, a sponge retains its positive charge longer than conventional cotton or wool cloths. This is likely due to its smoother surface, which reduces the chance of discharge through airborne negative ions.

When the charged rod or cloth is removed, the indicator returns to a gray circle, representing zero net charge. Similarly, when a glass rod is rubbed with a piece of leather, the glass becomes positively charged, while the leather becomes negatively charged.

**Physical background:** The external charge polarizes the “plate” of the electroscope, resulting in induced charges. The sign of the induced charge depends on the sign of the external one.

---

## Charge Transfer

If a pre-charged conductor (e.g., a metal sphere) placed on an insulating base touches the electroscope's plate, the reading does not return to zero after removing the conductor — indicating that some of the charge has transferred to the measuring capacitor.

This effect can also be demonstrated without direct contact. An aluminum soda can standing on an insulating surface can be placed near the sensor. When a negatively charged PVC rod is brought close to the plate, charge is induced on the can and may remain for an extended period. Because the can has greater capacitance than the plate, more charge can be accumulated on it — even in multiple steps by recharging the rod and repeating the process.

---

## Electrostatic Charging by Separation (Charge Separation)

In this experiment, charge is created via induction. A negatively charged PVC rod is brought near a neutral aluminum soda can, without making contact. The sensor already detects the redistribution of charge (green circle). Then, the side of the can opposite the rod is touched with a finger, allowing electrons to leave the can through the body to the ground.

After the finger is lifted and the rod is removed, the can remains positively charged, which is indicated by a red circle on the display.

---

## Demonstrating the Photoelectric Effect

This experiment effectively demonstrates the sensitivity and capabilities of the charge measurement device. Before starting, the concave base of an aluminum soda can is polished with fine abrasive cloth. The can is then negatively charged (e.g., by touching it with a rubbed PVC rod). The can retains its charge for a long time — several minutes — especially under low humidity conditions.

After charging, the can’s base is illuminated with a low-power UV light source (wavelength 254 nm). This exposure causes the can to gradually lose its negative charge, which is clearly visible on the display.

**Physical background:** The energy of 254 nm UV photons is approximately 4.89 eV, which exceeds the work function of aluminum (4.28 eV). Therefore, the photons can eject electrons from the metal surface, reducing its negative charge.

It is essential that the aluminum surface is clean and oxide-free, as the natural Al₂O₃ layer significantly reduces or completely blocks electron emission.

**Note:** If the can is initially charged positively (e.g., via charge separation), UV illumination will not result in any noticeable charge change. This confirms that it is the negative charge carriers (electrons) that are emitted due to the photoelectric effect — positive carriers are not released under light exposure.

---

## Investigating the Time-Dependent Discharge Due to the Photoelectric Effect

This experiment explores how the negative charge of an aluminum soda can decreases over time when exposed to a low-power 254 nm UV light source.

The values measured by the charge sensor can be plotted in real time using a graphical software tool such as [**SerialPlot v0.13**](https://github.com/hyOzd/serialplot) which is capable of displaying COM-port data over time.

**Note:** It is crucial to turn on the UV source before starting the measurement. Sudden changes in light intensity at the beginning may distort the results.
