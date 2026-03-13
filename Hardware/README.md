# OxTech MCM Hardware Design Files

Hardware design resources for integrating the OxTech MCM (Multi-Connectivity Module) into your product.

## Module Overview

| Parameter | Value |
|-----------|-------|
| **Part Number** | OxTech MCM LK8 |
| **SoC** | Silicon Labs EFR32MG24 |
| **Radio** | Semtech LR1110 (LoRa + GNSS) |
| **Protocols** | LoRaWAN, Amazon Sidewalk (BLE/FSK/CSS), BLE 5.3 |
| **Dimensions** | 15 mm x 15 mm x 2.5 mm (LGA) |
| **Pin Count** | 59 pads (LGA) |
| **Operating Voltage** | 1.8V - 3.6V |

## Contents

### `Altium/`
Altium Designer library files for schematic capture and PCB layout.

| File | Description |
|------|-------------|
| `MCM-LK8-Schematic.SchLib` | Schematic symbol with all 59 pins |
| `MCM-LK8-Footprint.PcbLib` | PCB footprint — pad layout, courtyard, and silkscreen |
| `MCM-LK8-Integrated.LibPkg` | Integrated library package linking symbol and footprint |

**Compatibility:** Altium Designer 20 and later.

**Usage:** Add the `.SchLib` and `.PcbLib` to your Altium project libraries, or install the `.LibPkg` as an integrated library.

### `3D-Models/`
3D STEP model for mechanical integration and clearance checks.

| File | Description |
|------|-------------|
| `MCM-LK8-3D-Model.step` | 3D model of the MCM LK8 module |

Import into your MCAD tool (SolidWorks, Fusion 360, etc.) or link to the Altium footprint for 3D board visualization.

## Documentation

For full electrical specifications, pin definitions, and design guidelines, see the datasheet:

- [MCM Datasheet (short)](../Documents/MCM_datasheet_short.pdf)
- [MCM Demo Guide](../Documents/MCM_demo_guide.pdf)
- [MCM Standalone Guide](../Documents/mcm_standalone_guide.pdf)

## Questions

For technical support, custom integration assistance, or access to additional design files:

- **Email:** support@oxit.com
- **Web:** [oxit.com/contact](https://oxit.com/contact)
