# MCM Playground

This repository contains resources, examples, and tools for working with the Multi-protocol Communication Module (MCM) with ESP32-S3 Feather from Adafruit as host.

**Note:** This demo example code and documentation is for MCM Evk Gen2 Rev2 by default. To run the demo on other versions of the Evk, some changes in the Arduino code may be needed.

## Repository Structure

- `Documents/` - Contains general documentation
- `Examples/` - Contains example projects demonstrating MCM implementation
  - `ArduinoESP32S3FeatherMultiProtocol/` - Multi-protocol example for ESP32-S3 Feather
  - `ArduinoESP32S3FeatherSidewalk/` - Sidewalk-specific example for ESP32-S3 Feather
- `tools/` - Utility tools and scripts for MCM development


## Examples

### Multi-protocol Example

The `ArduinoESP32S3FeatherMultiProtocol` example demonstrates how to implement multiple communication protocols on an ESP32-S3 Feather board. This example includes:

- Implementation of multiple wireless protocols
- Configuration management
- Basic communication examples

### Sidewalk Example

The `ArduinoESP32S3FeatherSidewalk` example focuses specifically on Amazon Sidewalk implementation on the ESP32-S3 Feather board.

## Getting Started

1. Clone this repository
2. Navigate to the desired example directory
3. Follow the specific example's documentation in its respective directory
4. Use the provided libraries and tools for development

## Requirements

- ESP32-S3 Feather board
- Arduino IDE or compatible development environment
- Required libraries (specified in each example)

## Documentation

Each example contains its own documentation in the respective `docs/` directory with detailed setup instructions and usage guidelines.

## Version Management

Version information for MCM releases can be found in the `mcm_release_versions.json` file within each example directory.

## Support

For support and additional information, please refer to the documentation or raise an issue in the repository.
