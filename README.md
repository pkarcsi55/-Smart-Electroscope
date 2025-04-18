# Smart Qmeter

A **Smart Qmeter** egy ESP32 (WeMos LOLIN32 Lite) mikrokontrollerre épülő, egyszerű, mégis sokoldalú töltésmérő eszköz. A célja, hogy demonstrálja a töltésmérés és a visszajelzés működését mikrokontrolleres környezetben.

## 🔧 Főbb jellemzők

- Analóg töltésszenzorral mért feszültség értékelése
- Töltés számítása kalibrált Cf kondenzátor alapján
- Automatikus nullázás (AutoZero gombbal)
- Váltható méréstartomány (pl. 520 pF és 4,7 nF Cf)
- Bluetooth kommunikáció (`Qmeter` néven jelenik meg)
- Soros kommunikáció (9600 baud)
- LED-es visszajelzés (pozitív = piros, negatív = zöld)
- PWM-es fényerő-szabályzás a töltés nagyságától függően

## 📷 Hardver követelmények

- WeMos LOLIN32 Lite (ESP32)
- DG444 analóg multiplexer
- Cf kondenzátorok (tipikusan: 520 pF és 4,7 nF)
- LED-ek: 2 piros, 2 zöld (PWM vezérelve)
- AutoZero nyomógomb
- Analóg töltésmérő elektronika (pl. erősítő, integrátor)

## 📂 Fájlok

- `Smart_Qmeter.ino` – a fő programfájl (Arduino IDE-hez)
- `README.md` – ez a leírás

## 🔌 Parancsok (Bluetooth vagy Soros porton keresztül)

| Parancs | Funkció              |
|--------|----------------------|
| `Z`    | nullázás (AutoZero)  |
| `+`    | pozitív irány beállítása |
| `-`    | negatív irány beállítása |
| `H`    | nagy érzékenység (pl. 0.9 nF Cf) |
| `L`    | kis érzékenység (pl. 10 nF Cf)  |

## 💡 LED visszajelzés

A töltés polaritása és nagysága alapján a LED-ek különböző fényerővel világítanak:
- **Zöld**: negatív töltés
- **Piros**: pozitív töltés
- Kis töltés esetén halvány fény, nagy töltésnél teljes fényerő

## 📈 Működés röviden

1. A mikrokontroller az A27 bemenetről olvassa a feszültséget.
2. A program kiszámítja a Cf kondenzátoron fellépő töltést.
3. A töltés értéke továbbításra kerül Bluetooth-on és/vagy soros porton keresztül.
4. A LED-ek vizuálisan kijelzik az értéket.

---

📌 A projekt oktatási céllal készült, egyszerű kísérleti demonstrációhoz.  
Ha hasznosnak találod, csillagozd a repót, vagy nyugodtan forkolhatod!
