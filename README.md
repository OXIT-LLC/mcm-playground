# Oxit MCM Playground: Multi-Protocol IoT Development with Amazon Sidewalk & LoRaWAN

Welcome to the Oxit MCM Playground, a comprehensive developer environment of resources, examples, and tools for exploring and building IoT solutions using the OxTech Multi-Connectivity Module (MCM) as a network coprocessor (NCP) with the Adafruit ESP32-S3 Feather as the host. This repository provides everything you need to accelerate deployment across Amazon Sidewalk, LoRaWAN, and the Helium Network.

**Note:** The demo examples and documentation in this repository target the MCM Evk Gen2 Rev2 (G2R2). To run the demo on earlier or alternate revisions, minor adjustments in the Arduino code may be required.

---

## Quick Start

### Hardware Requirements
- ESP32-S3 (Adafruit Feather recommended)
- OxTech MCM Module (EFR32MG24 + SX1262)
- USB-C cable for programming

### Wiring

| ESP32-S3 Pin | MCM Pin | Function |
|--------------|---------|----------|
| GPIO 10 | RX | UART TX → MCM RX |
| GPIO 9 | TX | UART RX ← MCM TX |
| GPIO 14 | RESET | MCM hardware reset |
| 3.3V | VCC | Power |
| GND | GND | Ground |

### First Steps
1. Clone this repository
2. Open `Examples/ArduinoESP32S3FeatherMultiProtocol/` in Arduino IDE
3. Install required libraries (see example README)
4. Flash to your ESP32-S3
5. Open Serial Monitor at 115200 baud

---

## Overview: Transform IoT Development with Oxit's MCM

The OxTech MCM is a certified, energy-efficient IoT module that integrates:
- Amazon Sidewalk for secure, low-bandwidth mesh networking
- LoRaWAN for long-range, low-power connectivity
- BLE 5.3 for local device interaction
- Helium Network support for decentralized LPWAN options

Built on the EFR32MG24 SoC and Semtech SX1262 transceiver, and pre-provisioned for rapid onboarding, the MCM supports advanced applications in smart cities, utilities, asset tracking, and industrial automation.

---

## Supported Hardware

### MCM Module Variants
| Part Number | Radio | Protocol Support |
|-------------|-------|------------------|
| OxTech MCM Series 0 | SX1262 + EFR32MG24 | LoRaWAN + Sidewalk (BLE/FSK/CSS) |
| OxTech MCM LK8| LR1110 + EFR32MG24 | LoRaWAN + Sidewalk + GNSS |

### OXIT Development Kits
| Kit | Contents | Use Case |
|-----|----------|----------|
| MCM DevKit | MCM + ESP32-S3 Feather + antenna | Quick prototyping |

### Tested Host MCUs
| MCU | Board | Status |
|-----|-------|--------|
| ESP32-S3 | Adafruit Feather ESP32-S3 | Primary - fully tested |

### Compatible Gateways
Any gateway supporting Basic Station or Packet Forwarder:
- OXIT Carbon 8-Channel Gateway


### Protocol Notes
- **LoRaWAN and Sidewalk do NOT operate simultaneously** - switch between them using CLI commands or firmware configuration
- **Sidewalk link types (BLE, FSK, CSS)** - check firmware version for supported combinations

---

## Repository Structure

- `Documents/` - General documentation and implementation guides
- `Examples/` - Sample projects demonstrating core MCM capabilities
  - `ArduinoESP32S3FeatherMultiProtocol/` - Multi-protocol (Amazon Sidewalk CSS + LoRaWAN) example
  - `ArduinoESP32S3FeatherSidewalk/` - Sidewalk-specific (BLE, FSK, CSS) example
- `tools/` - Scripts and utilities for MCM configuration and deployment

---

## CLI Commands

Send commands via the ESP32's USB serial console using `oxit_cli`:

```
oxit_cli deveui <16-hex-chars>       # Set LoRaWAN DevEUI
oxit_cli joineui <16-hex-chars>      # Set LoRaWAN JoinEUI
oxit_cli appkey <32-hex-chars>       # Set LoRaWAN AppKey
oxit_cli protocol_switch <mode>      # Switch protocol (lorawan, sw_ble, sw_fsk, sw_css)
oxit_cli uplink_now                  # Send immediate uplink
oxit_cli reboot                      # Restart the MCM
oxit_cli erase                       # Erase stored credentials
?                                    # Show help
```

---

## Troubleshooting

### MCM Not Responding
```
Symptom: No response from mcm.print_version()
```
**Fixes:**
1. Check UART wiring (TX→RX, RX→TX, not TX→TX)
2. Verify baud rate is 9600
3. Try hardware reset: `mcm.reset()`

### LoRaWAN Join Fails
```
Symptom: JOIN_FAILED event after connection attempt
```
**Fixes:**
1. Verify credentials match your LNS registration (TTN, TTI, ChirpStack)
2. Check gateway is online and in range
3. Confirm regional parameters and sub-band (e.g., US915 FSB2, EU868)
4. 8-channel gateways may take time to receive on all channels

### Sidewalk Not Connecting
```
Symptom: SIDEWALK_STATUS shows "Not Registered"
```
**Fixes:**
1. Ensure device is provisioned with valid manufacturing page for your Amazon account
2. Use **BLE or FSK mode** for initial registration (CSS does not support registration)
3. Confirm you're in range of a Sidewalk gateway supporting your link type

> **Note:** Device registration happens via provisioning, NOT the Amazon Developer Console.

---

## Examples

### 1) Multi-protocol Example
The `ArduinoESP32S3FeatherMultiProtocol` example demonstrates:
- Multi-protocol integration of Amazon Sidewalk (CSS) + LoRaWAN
- Dynamic configuration and protocol switching
- Best for asset tracking, environmental monitoring, smart home

### 2) Amazon Sidewalk Example
The `ArduinoESP32S3FeatherSidewalk` example demonstrates:
- Amazon Sidewalk over BLE (short range), FSK (medium range), CSS (long range)
- AWS IoT Core connectivity
- Ideal for leak detection, geolocation, building safety

---

## Requirements

- Adafruit ESP32-S3 Feather Board
- OxTech MCM Evk G2R2 Board
- Arduino IDE or compatible IDE
- Required libraries (specified per example)
- AWS account for Sidewalk/LoRaWAN integration via AWS IoT Core (optional)

---

## Why Choose Oxit's MCM?

✓ **Multi-Network Flexibility:** Combine Amazon Sidewalk, LoRaWAN, and BLE in a single device

✓ **AWS Integration:** Native support for AWS IoT Core and Sidewalk

✓ **Battery-Optimized:** Designed for extended field deployments

✓ **Secure by Design:** Secure Vault™, encrypted provisioning, and OTA update readiness

---

## MCM Firmware & Libraries

The MCM module firmware and complete host library are available to customers and partners. Contact OXIT for:
- MCM firmware binary (.hex) for programming the module
- Full host library with additional examples
- Technical support and integration assistance

**Contact:** [oxit.com/contact](https://oxit.com/contact)

> This repository contains example code and documentation. Complete firmware requires direct access from OXIT.

---

## Support & Contribution

**Get Help**
- Refer to the included documentation
- Open an issue for technical questions
- Visit www.oxit.com or contact support@oxit.com for enterprise support

**Contribute**
1. Fork this repository
2. Create a branch: `git checkout -b feature/your-feature`
3. Commit and push your changes
4. Open a Pull Request

---

## About Oxit

Oxit is redefining IoT innovation through custom-engineered connectivity solutions. With deep expertise in LoRaWAN, Amazon Sidewalk, and full-stack IoT development, Oxit empowers developers and businesses to build secure, scalable, and future-ready applications.

**Headquartered in Charlotte, NC**
- Contact: sales@oxit.com | marketing@oxit.com
- Learn more at: https://www.oxit.com
