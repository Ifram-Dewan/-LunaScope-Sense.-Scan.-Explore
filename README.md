# -LunaScope-Sense.-Scan.-Explore
LunaScope — an Arduino UNO based multi-sensor exploration and environmental monitoring prototype inspired by NASA's Lunar Reconnaissance Orbiter (LRO).

# 🌙 LunaScope

### Arduino-Based Environmental Monitoring & Obstacle Detection System

**LunaScope** is an Arduino UNO-based multi-sensor exploration and environmental monitoring prototype inspired by the mission concept of NASA's **Lunar Reconnaissance Orbiter (LRO)**.

The project combines multiple sensors with servo-based scanning to create a small-scale experimental platform capable of monitoring environmental conditions, detecting obstacles, measuring light, and presenting real-time information on an OLED display.

> **Inspiration:** NASA's Lunar Reconnaissance Orbiter (LRO)
> **Platform:** Arduino UNO
> **Project Type:** Embedded Systems / Robotics / Environmental Monitoring
> **Status:** Prototype / Educational Project

---

## 🚀 Inspiration

LunaScope is inspired by the sensing and exploration philosophy of NASA's **Lunar Reconnaissance Orbiter (LRO)**.

NASA's LRO is a robotic spacecraft that has studied the Moon since 2009, producing detailed information about the lunar surface and environment. Its instruments support mapping, imaging, terrain analysis, and investigation of lunar environmental conditions. [NASA LRO Mission](https://science.nasa.gov/mission/lro/)

LunaScope brings a simplified version of this idea into an Arduino-based terrestrial prototype:

```text
LRO
 ↓
Observe
 ↓
Measure
 ↓
Analyze
 ↓
Support Exploration

LunaScope
 ↓
Sense
 ↓
Measure
 ↓
Process
 ↓
Display
```

LunaScope is **not a replica of LRO** and is not affiliated with or developed by NASA.

---

# 🎯 Project Objective

The main objective of LunaScope is to develop a compact embedded system capable of:

* 🌡️ Measuring temperature
* 💧 Measuring humidity
* 💨 Monitoring changes in air quality
* 🌧️ Detecting rain/water
* ☀️ Measuring ambient light
* 💡 Automatically controlling an LED
* 📡 Detecting nearby obstacles
* 🔄 Scanning the surroundings using servo motors
* 📺 Displaying sensor information in real time

---

# 🧠 System Overview

```text
                         ┌─────────────────────┐
                         │     Arduino UNO     │
                         │   Main Controller   │
                         └──────────┬──────────┘
                                    │
          ┌─────────────┬───────────┼───────────┬──────────────┐
          │             │           │           │              │
          ▼             ▼           ▼           ▼              ▼
       HC-SR04        MQ-135      DHT22       Rain           LDR
      Ultrasonic    Air Quality   Temp &      Sensor        Light
      Detection                  Humidity
          │
          ▼
      Servo Scanning
       ┌─────┴─────┐
       ▼           ▼
    SG90 #1      SG90 #2
   Horizontal    Vertical
      Scan          Tilt
       
          ┌───────────────────────────────┐
          │                               │
          ▼                               ▼
       OLED Display                     LED
     Sensor Readings                Auto Lighting
```

---

# 🔧 Hardware Components

| Component      | Function                               |
| -------------- | -------------------------------------- |
| Arduino UNO    | Main microcontroller                   |
| HC-SR04        | Ultrasonic obstacle/distance detection |
| SG90 Servo #1  | Horizontal sensor scanning             |
| SG90 Servo #2  | Vertical sensor positioning            |
| MQ-135         | Air-quality/gas response sensing       |
| DHT22          | Temperature and humidity measurement   |
| Rain Detector  | Rain/water detection                   |
| LDR            | Ambient light sensing                  |
| LED            | Automatic light indicator              |
| 1.3" OLED/QLED | Real-time data display                 |
| 220Ω Resistor  | LED current limiting                   |
| Breadboard     | Circuit prototyping                    |
| Jumper Wires   | Electrical connections                 |

---

# 🔌 Pin Configuration

| Component       | Signal Pin | Arduino UNO |
| --------------- | ---------- | ----------- |
| DHT22           | DATA       | D2          |
| LED             | Signal     | D6          |
| SG90 Horizontal | Signal     | D5          |
| SG90 Vertical   | Signal     | D7          |
| HC-SR04         | TRIG       | D8          |
| HC-SR04         | ECHO       | D9          |
| MQ-135          | AO         | A0          |
| Rain Sensor     | AO         | A1          |
| LDR             | AO         | A2          |
| OLED            | SDA        | A4          |
| OLED            | SCL        | A5          |

### Power

```text
Sensors → 5V / GND
OLED    → 5V / GND
Servos  → External regulated 5V recommended
Arduino → USB / suitable power source
```

For external servo power, connect the external supply GND to Arduino GND.

---

# ⚙️ How LunaScope Works

## 1. 🌡️ Temperature & Humidity

The DHT22 sensor measures:

```text
Temperature → °C
Humidity    → %
```

The Arduino processes these values and displays them on the OLED.

---

## 2. 💨 Air Quality

The MQ-135 provides an analog response associated with changes in gases/air quality.

```text
MQ-135
   ↓
Analog Reading
   ↓
Arduino A0
   ↓
OLED / Serial Monitor
```

The raw MQ-135 reading is used as a sensor value in this prototype.

> The raw analog value should not be interpreted directly as a calibrated AQI or gas concentration without proper calibration.

---

## 3. 🌧️ Rain Detection

The rain sensor detects water/rain on its sensing surface.

```text
Rain Sensor
     ↓
Analog Signal
     ↓
Arduino A1
     ↓
Rain Status / Value
```

---

## 4. ☀️ Light Detection

The LDR measures changes in surrounding light intensity.

```text
Light
  ↓
LDR
  ↓
Analog Reading
  ↓
Arduino A2
```

---

## 5. 💡 Automatic Lighting

The LDR is also used to control the LED.

```text
                LDR
                 ↓
          Measure Light
                 ↓
        ┌────────┴────────┐
        ↓                 ↓
      Dark              Bright
        ↓                 ↓
     LED ON             LED OFF
```

The threshold can be adjusted according to the actual LDR readings.

---

# 📡 6. Obstacle Detection

The HC-SR04 measures the distance between LunaScope and nearby objects.

```text
HC-SR04
   ↓
Ultrasonic Pulse
   ↓
Object
   ↓
Echo
   ↓
Distance Calculation
```

The prototype uses a configurable obstacle threshold.

Example:

```text
Distance < 20 cm
       ↓
Obstacle Detected
```

---

# 🔄 7. Servo-Based Scanning

Two SG90 servo motors provide positioning for the ultrasonic sensor.

### Horizontal Servo

```text
        LEFT       CENTER       RIGHT
         30°         90°         150°
          ↙           ↓           ↘
             HC-SR04
```

The system checks:

```text
Left Distance
Center Distance
Right Distance
```

This provides a simple directional environmental scan.

### Vertical Servo

The second SG90 provides vertical positioning/tilting of the sensor assembly.

---

# 📺 OLED Display

LunaScope displays sensor data on a 1.3-inch I2C OLED/QLED display.

### Example Screen 1

```text
ENVIRONMENT MONITOR
--------------------
Temp: 28.5 C
Humidity: 72.4%
Air: 385
Rain: 650
```

### Example Screen 2

```text
SMART MONITOR
--------------------
Light: 320
Center: 18cm
Left: 42cm
Right: 31cm
```

---

# 💻 Software

The project is programmed using:

* Arduino IDE
* C/C++
* Arduino Servo Library
* DHT Sensor Library
* U8g2 OLED Library
* Wire/I2C communication

### Required Libraries

```text
DHT sensor library
Adafruit Unified Sensor
U8g2
Servo
Wire
```

---

# 📁 Project Structure

```text
LunaScope/
│
├── README.md
│
├── src/
│   └── LunaScope.ino
│
├── docs/
│   ├── circuit-diagram.png
│   ├── system-block-diagram.png
│   └── project-report.pdf
│
├── images/
│   ├── lunascope-front.jpg
│   ├── lunascope-circuit.jpg
│   ├── lunascope-display.jpg
│   └── lunascope-testing.jpg
│
├── LICENSE
│
└── .gitignore
```

---

# 📊 Features

| Feature                       | Status         |
| ----------------------------- | -------------- |
| Temperature Monitoring        | ✅              |
| Humidity Monitoring           | ✅              |
| Air Quality Sensor            | ✅              |
| Rain Detection                | ✅              |
| Light Detection               | ✅              |
| Automatic LED                 | ✅              |
| Ultrasonic Distance Detection | ✅              |
| Horizontal Servo Scanning     | ✅              |
| Vertical Servo Positioning    | ✅              |
| OLED Data Display             | ✅              |
| Real-Time Serial Monitoring   | ✅              |
| Autonomous Wheel Movement     | 🔄 Future Work |

---

# 🧪 Example Workflow

```text
START
  ↓
Initialize Sensors
  ↓
Initialize OLED
  ↓
Initialize Servos
  ↓
Read Temperature
  ↓
Read Humidity
  ↓
Read Air Quality
  ↓
Read Rain Sensor
  ↓
Read Light Level
  ↓
Control LED
  ↓
Scan Left
  ↓
Scan Center
  ↓
Scan Right
  ↓
Display Results
  ↓
Repeat
```

---

# 🔬 Applications

LunaScope can be used as an educational prototype for:

* Embedded systems
* Robotics
* Environmental monitoring
* Sensor integration
* IoT concepts
* Autonomous sensing
* STEM education
* Space-inspired engineering projects

---

# 🚀 Future Improvements

The project can be extended with:

* [ ] DC motors for autonomous movement
* [ ] L298N motor driver
* [ ] Bluetooth/Wi-Fi connectivity
* [ ] Mobile application
* [ ] GPS module
* [ ] SD-card data logging
* [ ] Real-time web dashboard
* [ ] Solar-powered operation
* [ ] Camera module
* [ ] AI-based object detection
* [ ] Cloud sensor monitoring
* [ ] Battery monitoring
* [ ] Improved environmental calibration

---

# 🛰️ Inspiration & Technical Context

NASA's Lunar Reconnaissance Orbiter uses scientific instruments to study the lunar surface and environment. Among its instruments are LROC for high-resolution imaging, LOLA for lunar topography and surface roughness, Diviner for thermal measurements, LEND for neutron/radiation-related measurements, and Mini-RF for radar observations.

LunaScope does **not reproduce these scientific instruments**. Instead, it takes the broader concept of a multi-sensor robotic exploration platform and implements a small educational prototype using commercially available Arduino-compatible sensors.

---

# 👨‍💻 Project Information

**Project Name:** LunaScope

**Platform:** Arduino UNO

**Category:** Embedded Systems / Robotics / Environmental Monitoring

**Inspiration:** NASA Lunar Reconnaissance Orbiter (LRO)

**Development Environment:** Arduino IDE

**Project Status:** Prototype

---

# 📜 License

This project is intended for educational and research purposes.

You are free to study, modify, and extend the project with appropriate attribution.

---

# ⭐ Acknowledgement

This project is inspired by NASA's Lunar Reconnaissance Orbiter mission and the broader concept of robotic exploration through remote sensing.

NASA LRO information:

https://science.nasa.gov/mission/lro/

NASA describes LRO as an active lunar-orbiting mission that has been studying the Moon since 2009 and providing data useful for science and future exploration.

---

## 🌙 LunaScope

**Sense. Scan. Explore.**

> *A small prototype inspired by a big idea — robotic exploration through intelligent sensing.*
