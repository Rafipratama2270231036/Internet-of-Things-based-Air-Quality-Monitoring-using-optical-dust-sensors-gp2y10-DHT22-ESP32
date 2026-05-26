# 🌫️ ESP32 Multi-Sensor Air Quality Monitoring

[![ESP32](https://img.shields.io/badge/ESP32-WROOM-green)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Arduino-IDE-blue)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> **Bagian dari Skripsi:** *Prediksi Kualitas Udara berbasis Data Sensor Internet of Things menggunakan Algoritma Time Series*

---

## 📋 Deskripsi

Firmware ESP32 pengirim (*sender node*) untuk sistem monitoring kualitas udara berbasis IoT. Perangkat ini mengakuisisi data **PM2.5**, **suhu**, dan **kelembaban** secara real-time, kemudian mengirimkannya ke ESP32 penerima via komunikasi **UART Serial2** untuk diproses lebih lanjut termasuk prediksi menggunakan algoritma **Time Series**.

---

## 🔧 Hardware

| Komponen | Tipe | Fungsi |
|----------|------|--------|
| Mikrokontroler | **ESP32-WROOM-32** | Akuisisi & pengiriman data |
| Sensor Debu | **Sharp GP2Y1010AU0F** | Konsentrasi PM2.5 (µg/m³) |
| Suhu & Kelembaban | **DHT22 (AM2302)** | Suhu (°C) & Kelembaban (%RH) |
| Komunikasi | **UART Serial2** | TX: GPIO1, RX: GPIO3 |

---

## 🔌 Wiring Diagram

### GP2Y1010AU0F → ESP32
| Sensor | ESP32 |
|--------|-------|
| LED (putih) | **GPIO2** |
| VO (hitam) | **GPIO34** (ADC) |
| GND | **GND** |
| VCC | **5V / VIN** |

### DHT22 → ESP32
| Sensor | ESP32 |
|--------|-------|
| DATA | **GPIO26** |
| VCC | **3.3V** |
| GND | **GND** |

---

## 📡 Format Data

Data dikirim setiap **2 detik** dalam format CSV:
PM2.5,Suhu,Kelembaban
235,27.5,61.3
240,27.5,61.2
211,27.5,61.3


| Field | Satuan | Contoh |
|-------|--------|--------|
| PM2.5 | µg/m³ | `235` |
| Suhu | °C | `27.5` |
| Kelembaban | %RH | `61.3` |

---

## ⚙️ Fitur Firmware

- ✅ **Auto-baseline calibration** saat startup
- ✅ **Calibration factor** untuk akurasi sensor
- ✅ **Batasan PM2.5** (0–500 µg/m³)
- ✅ **Running average** noise filter
- ✅ **Error handling** DHT22 (NaN → 0)
- ✅ **Debug output** via Serial Monitor

---

## 📚 Library

| Library | Author |
|---------|--------|
| **GP2YDustSensor** | [Lucian Sabo](https://github.com/luciansabo/GP2YDustSensor) |
| **DHT Sensor Library** | [Adafruit](https://github.com/adafruit/DHT-sensor-library) |
