# 📋 RFID-Based Attendance System using NodeMCU (ESP8266)

This project provides a contactless **attendance system** using **RFID tags** and **NodeMCU ESP8266**. When a user taps their RFID card on the RC522 reader, their ID is read and sent via Wi-Fi to a **Google Sheet** using **Google Apps Script**.

## 🚀 Features
- ✅ RFID-based contactless attendance
- 🌐 Real-time data logging to Google Sheets
- 📡 Uses SPI protocol for RFID communication
- 🔒 Secure HTTP(S) requests using BearSSL
- 🔊 Buzzer feedback on card scan
- 🔌 Minimal hardware with Wi-Fi capability

## 🧩 Hardware Components
| Component           | Quantity |
|---------------------|----------|
| NodeMCU ESP8266     | 1        |
| RC522 RFID Reader   | 1        |
| RFID Tags/Cards     | 2+       |
| Buzzer              | 1        |
| Breadboard + Wires  | As needed |

## 🔌 Circuit Connections
| RC522 Pin | NodeMCU Pin | Description      |
|-----------|-------------|------------------|
| SDA       | D4 (GPIO2)  | SS (Slave Select)|
| SCK       | D5 (GPIO14) | SPI Clock        |
| MOSI      | D7 (GPIO13) | SPI MOSI         |
| MISO      | D6 (GPIO12) | SPI MISO         |
| RST       | D3 (GPIO0)  | Reset            |
| GND       | GND         | Ground           |
| 3.3V      | 3.3V        | Power Supply     |
| Buzzer    | D2 (GPIO4)  | Audio feedback   |

## 🔧 Software Setup

### 1. Install Arduino Libraries
Install the following libraries from the Library Manager:
- `MFRC522`
- `ESP8266WiFi`
- `ESP8266HTTPClient`
- `WiFiClientSecure`

### 2. Setup Google Sheet
1. Create a new Google Sheet.
2. Go to **Extensions > Apps Script**.
3. Paste and deploy a **Google Apps Script** that accepts GET requests and appends data.
4. Copy the deployment URL and use it in your code (`sheet_url`).

### 3. Upload Code
- Configure your Wi-Fi credentials in the code:
  ```cpp
  #define WIFI_SSID "YourWiFiName"
  #define WIFI_PASSWORD "YourWiFiPassword"


🙌 Credits
Made with 💙 by:

Anshu Manoj Jamare
Zaid Khan
Anushka Pote
Electronics & Telecommunication, Ramdeobaba College
With 💙 for IoT and Automation Projects
