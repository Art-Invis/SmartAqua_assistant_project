# 🌊 SmartAqua_assistant_ – IoT Course Project

📖 **About This Repository**
This repository contains a complete implementation of the **SmartAqua\_assistant\_ project**, developed as part of microcontroller laboratory work.
The system is based on the **Wemos D1 Mini (ESP8266)** and demonstrates:

* Integration of **sensors** (temperature, water level)
* Communication protocols (**I²C**, **1-Wire**, UART)
* Cloud data exchange with **Firebase**
* A **dashboard** for visualization and interaction
* Clear schematics and diagrams for both functional and circuit-level understanding

The project highlights IoT design principles, embedded programming in Arduino C++, and system-level thinking.

---

## 🗂 Repository Structure

```
├── dashboard/               # Web dashboard for monitoring & control
│   ├── js/                  # JavaScript logic
│   ├── styles/              # CSS styles
│   └── index.html           # Entry point for the dashboard
│
├── firmware/                # Microcontroller firmware
│   └── src/                 # Arduino C++ source code
│
├── shematics/               # Diagrams & schematics
│   ├── block_diagram.jpg
│   ├── circuit_diagram.jpg
│   └── functional_diagram.jpg
│
├── README.md                # You're reading it
└── .gitignore
```

---

## 🚀 Technologies Used

| Component                   | Highlights                      |
| --------------------------- | ------------------------------- |
| **ESP8266 (Wemos D1 Mini)** | Core microcontroller with Wi-Fi |
| **Arduino IDE (C++)**       | Firmware development            |
| **I²C & 1-Wire**            | Communication protocols         |
| **Firebase**                | Cloud storage & synchronization |
| **HTML/CSS/JS**             | Web dashboard interface         |
| **KiCad**                   | Circuit design & schematics     |

---

## 📎 Project Features

* **Wi-Fi + Firebase connectivity** for remote control and data sync
* **OLED display (I²C)** for real-time system status
* 🌊 **Water level sensor** (analog) monitoring
* 🌡 **Temperature sensor DS18B20** integration
* 🕒 Real-time & scheduled event display
* Splash screen, icons, and smooth display transitions
* Interactive **dashboard** for visual monitoring and control

---

## ✍️ How to Use

1. Clone the repo:

   ```bash
   git clone https://github.com/.../SmartAqua_assistant_.git
   ```

2. Flash the firmware:

   * Open `firmware/src/` in Arduino IDE
   * Configure **Wi-Fi credentials** and **Firebase keys**
   * Upload to Wemos D1 Mini

3. Open the dashboard:

   * Navigate to `dashboard/index.html` in a browser
   * Ensure the ESP8266 is running and connected to Firebase

4. Review diagrams:

   * `shematics/block_diagram.jpg`
   * `shematics/circuit_diagram.jpg`
   * `shematics/functional_diagram.jpg`

---

## 👨‍🎓 Attribution

Developed and maintained by **Artem Sokol**
Educational purpose only.

© 2025 Artem. All sensors calibrated, all rights respected.
