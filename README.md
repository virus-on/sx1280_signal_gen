# SX1280 2.4GHz Signal Generator

A web-controlled 2.4GHz signal and Continuous Wave (CW) generator built on the HPXGRC 2.4 ELRS Module. 

This tool is designed to provide an easy-to-use interface for RF testing, debugging, and emission generation directly from your smartphone or PC over a dedicated Wi-Fi Access Point.

## Features
* **Standalone Wi-Fi AP:** Hosts its own `SX1280 Signal Generator` network.
* **Web UI:** Responsive, slider-based control interface optimized for both mobile and desktop.
* **Real-time Tuning:** Instant adjustments for Frequency (2400 MHz - 2500 MHz) and Output Power (-17 to 12).
* **Hardware Amplifier Control:** Toggle support for external PA/LNA (RX/TX enable pins).
* **Signal Output:** Direct transmission mode for SDR testing, antenna tuning, or active emission.

## Hardware Requirements
* **2.4 ELRS module:** HPXGRC 2.4 ELRS Module.
* **External Programmer:** CH341A (or similar USB-to-TTL serial adapter) for initial flashing.

---

## Wiring & Flashing

### Connecting a CH341A to an HPXGRC 2.4 ELRS Module
To flash the firmware onto an HPXGRC 2.4GHz ELRS module using a CH341A adapter, configure the hardware as follows:

**1. CH341A Configuration:**
* Ensure the voltage/mode selection jumper on the CH341A is placed **between pins 2 and 3**.

**2. Wiring Pinout:**
| CH341A Adapter | HPXGRC ELRS Module | Description |
| :--- | :--- | :--- |
| `5V` | `V` | Power supply |
| `GND` | `G` | Common ground |
| `TXD` | `R` | Transmit to Receive |
| `RXD` | `T` | Receive to Transmit |

*Note: To enter bootloader mode for flashing the ESP8255, you usually need to hold down the boot button on the receiver while plugging it in, or bridge the boot pad to ground depending on the exact hardware revision.*

---

## Build and Installation
This project is configured for **PlatformIO**.

1. Clone this repository.
2. Open the project folder in VSCode with the PlatformIO extension installed.
3. Connect your ELRS module via the CH341A programmer.
4. Click **Build** and then **Upload**.

*Important: The `platformio.ini` uses `board_build.flash_mode = dout` to free up the internal SPI flash pins (GPIO 9 and 10) so they can be safely used to control the radio's RX/TX enable lines without crashing the microcontroller.*

## Usage
1. Power on the module. The onboard LED will breathe or blink to indicate its current state.
2. Connect your phone or PC to the device's Wi-Fi network:
   * **SSID:** `SX1280 Signal Generator`
   * **Password:** *(Open Network)*
3. Open a web browser and navigate to `http://192.168.4.1`.
4. Use the interface to set your desired parameters and toggle the RF transmission.