# Oxit MCM Playground: Multi-Protocol IoT Development with Amazon Sidewalk & LoRaWAN

Welcome to the Oxit MCM Playground, a comprehensive developer environment of resources, examples, and tools for exploring and building IoT solutions using the OxTech Multi-Connectivity Module (MCM) as a network coprocessor (NCP) with the Adafruit ESP32-S3 Feather as the host. This repository provides everything you need to accelerate deployment across Amazon Sidewalk, LoRaWAN, and the Helium Network.

**Note:** The demo examples and documentation in this repository target the MCM Evk Gen2 Rev2 (G2R2). To run the demo on earlier or alternate revisions, minor adjustments in the Arduino code may be required.

## Overview: Transform IoT Development with Oxit's MCM
The OxTech MCM is a certified, energy-efficient IoT module that integrates:
- Amazon Sidewalk for secure, low-bandwidth mesh networking
- LoRaWAN for long-range, low-power connectivity
- BLE 5.3 for local device interaction
- Helium Network support for decentralized LPWAN options

Built on the EFR32MG24 SoC and Semtech SX1262 transceiver, and pre-provisioned for rapid onboarding, the MCM supports advanced applications in smart cities, utilities, asset tracking, and industrial automation.

## Repository Structure

- `Documents/` - General documentation and implementation guides
- `Examples/` - Sample projects demonstrating core MCM capabilities
  - `ArduinoESP32S3FeatherMultiProtocol/` - Multi-protocol (Amazon Sidewalk CSS + LoRaWAN) example for ESP32-S3 Feather
  - `ArduinoESP32S3FeatherSidewalk/` - Sidewalk-specific (Amazon Sidewalk BLE, FSK, BLE) example for ESP32-S3 Feather
- `tools/` - Scripts and utilities for MCM configuration, development, and deployment

## Examples

### 1) Multi-protocol Example

The `ArduinoESP32S3FeatherMultiProtocol` example demonstrates how to implement multiple communication protocols on an ESP32-S3 Feather board. This example includes:

- Multi-protocol integration of Amazon Sidewalk (CSS) + LoRaWAN
- Dynamic Configuration and protocol switching
- Basic communication examples
- Best for asset tracking, environmental monitoring, and smart home use cases

### 2) Amazon Sidewalk Example

The `ArduinoESP32S3FeatherSidewalk` example focuses specifically on Amazon Sidewalk implementation on the ESP32-S3 Feather board. This example includes:
- Amazon Sidewalk over BLE (short range), FSK (medium range), and CSS (long range)
- AWS IoT Core connectivity for Amazon Sidewalk
- Ideal for leak detection, geolocation, and building safety systems

## Getting Started

1. Clone this repository (git clone https://github.com/oxit/mcm-playground.git)
2. Navigate to the preferred example directory
3. Follow the specific example's documentation in its respective directory
4. Install required libraries (listed in each README)
5. Use the Arduino IDE or a compatible environment to flash your ESP32-S3

## Requirements

- Adafruit ESP32-S3 Feather Board
- OxTech MCM Evk G2R2 Board
- Arduino IDE or compatible IDE
- Required libraries (specified per example)
- AWS account for Sidewalk and LoRaWAN integration via AWS IoT Core (optional)

## Documentation

Each example contains its documentation in the respective `docs/` directory with detailed setup instructions and usage guidelines.

## Version Management

Version information for MCM firmware releases can be tracked in each example's `mcm_release_versions.json` file to ensure consistent compatibility between hardware, firmware, and development tools.

## Why Choose Oxit's MCM?
✓ **Multi-Network Flexibility:** Combine Amazon Sidewalk, LoRaWAN, and BLE in a single device

✓ **AWS Integration:** Native support for AWS IoT Core and Sidewalk

✓ **Battery-Optimized:** Designed for extended field deployments

✓ **Secure by Design:** Secure Vault™, encrypted provisioning, and OTA update readiness

## Support & Contribution
**Get Help**
- For additional information, please refer to the included documentation
- Open an issue in the repository for technical questions
- Visit www.oxit.com or contact support@oxit.com for enterprise support

**Contribute**
We welcome contributions! To submit a feature or fix:
1. Fork this repository
2. Create a branch: git checkout -b feature/your-feature
3. Commit and push your changes
4. Open a Pull Request

## About Oxit
Oxit is redefining IoT innovation through custom-engineered connectivity solutions. With deep expertise in LoRaWAN, Amazon Sidewalk, and full-stack IoT development, Oxit empowers developers and businesses to build secure, scalable, and future-ready applications.

**Headquartered in Charlotte, NC**
- Contact: sales@oxit.com | marketing@oxit.com
- Learn more at: https://www.oxit.com
