# NeuroVigilX 🛡️
**IoT-Based Real-Time Health & Security Monitoring System**

> Detect · Protect · Alert

![ESP32](https://img.shields.io/badge/ESP32-Wokwi-blue)
![Node-RED](https://img.shields.io/badge/Backend-Node--RED-red)
![FlowFuse](https://img.shields.io/badge/Cloud-FlowFuse-orange)
![Twilio](https://img.shields.io/badge/Alerts-Twilio%20WhatsApp-green)

---

## 🔍 Overview

NeuroVigilX is a real-time IoT health monitoring system that tracks heart rate,
proximity distance, and manual SOS triggers — streaming live data to a cloud
dashboard and dispatching WhatsApp emergency alerts via Twilio when vitals go critical.

---

## 🏗️ Architecture

ESP32 (Wokwi Simulator)
├── Potentiometer  →  Heart Rate  (pin 34)
├── HC-SR04        →  Distance    (pins 5/18)
├── Pushbutton     →  SOS         (pin 4)
└── Buzzer         →  Emergency   (pin 19)
↓  HTTP POST /secure-sos every 2s
Node-RED on FlowFuse Cloud
├── POST /secure-sos  →  store data + trigger WhatsApp
├── GET  /get-data    →  serve live data to dashboard
└── GET  /ping        →  health check
↓  polling every 2s
Live Dashboard (dashboard/index.html)
├── Real-time heart rate chart (last 20 readings)
├── Threat level index
├── Emergency alert banner
└── Device telemetry panel

-----

## ⚡ Features

| Feature | Details |
|---|---|
| 💓 Heart Rate Monitor | Analog potentiometer mapped 60–180 bpm |
| 📏 Proximity Sensor | HC-SR04 ultrasonic — triggers at < 15cm |
| 🆘 Manual SOS | Red pushbutton forces emergency state |
| 🔐 Token Auth | Bearer token validates every POST request |
| ☁️ Cloud Backend | Node-RED on FlowFuse — 3 REST endpoints |
| 📊 Live Dashboard | HTML/JS polls every 2s, zero dependencies |
| 📱 WhatsApp Alert | Twilio API — 60s cooldown between alerts |
| 🔁 Auto Retry | 3 retries with 3s gap + 20s timeout |

---

🚨 NEUROVIGILX EMERGENCY!
Heart Rate: 155 bpm
Distance: 8 cm
SOS Button: YES
Location: 12.9716
Time: 26/5/2026, 7:30:00 AM



