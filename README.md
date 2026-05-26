# 🌫️ ESP32 Multi-Sensor Air Quality Monitoring

[![ESP32](https://img.shields.io/badge/ESP32-WROOM-green)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Arduino-IDE-blue)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> **Bagian dari Skripsi:** *Prediksi Kualitas Udara berbasis Data Sensor IoT menggunakan Algoritma Time Series*

---

## 📋 Deskripsi

Firmware ESP32 pengirim data kualitas udara real-time menggunakan sensor **GP2Y1010AU0F** (PM2.5) dan **DHT22** (suhu & kelembaban). Data dikirim via **UART Serial2** ke ESP32 penerima untuk diproses lebih lanjut — termasuk prediksi Time Series.

---

## 🔧 Hardware

| Komponen | Tipe |
|----------|------|
| Mikrokontroler | ESP32-WROOM-32 |
| Sensor Debu | Sharp GP2Y1010AU0F |
| Suhu & Kelembaban | DHT22 (AM2302) |
| Komunikasi | UART Serial2 (TX: GPIO1, RX: GPIO3) |

---

## 🔌 Wiring

| GP2Y1010AU0F | ESP32 |
|--------------|-------|
| LED (putih) | GPIO2 |
| VO (hitam) | GPIO34 |
| GND | GND |
| VCC | 5V/VIN |

| DHT22 | ESP32 |
|-------|-------|
| DATA | GPIO26 |
| VCC | 3.3V |
| GND | GND |

---

## 📡 Format Data
