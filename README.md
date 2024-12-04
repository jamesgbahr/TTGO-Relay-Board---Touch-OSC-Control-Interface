# TTGO Relay Board - TouchOSC Control Interface

This project leverages the LILYGO® TTGO T-Relay ESP32 board to control its 4 relays via the TouchOSC app using Open Sound Control (OSC). It's ideal for home automation, lighting control, and other IoT applications requiring wireless relay management.

## Features

- **WiFi Connectivity**: Uses the TTGO T-Relay board's ESP32 for seamless wireless control.
- **TouchOSC Integration**: Control the relays via a custom layout in the TouchOSC app.
- **Dynamic Timing**: Relay toggling is based on OSC fader values, offering flexibility.
- **Multi-Relay Support**: Controls all 4 relays on the TTGO T-Relay board.
- **Debugging Support**: Serial output for monitoring OSC messages and relay activity.

## Components Required

- **TTGO T-Relay ESP32 Board** ([Product Page](https://lilygo.cc/products/t-relay))
- **WiFi Network**
- **TouchOSC App** (Available for iOS and Android)
- USB-C cable for programming and power

## Installation

### 1. Set Up Arduino IDE

1. Install the [ESP32 Board Package](https://arduino-esp8266.readthedocs.io/).
2. Install required libraries:
   - `WiFi`
   - `WiFiUdp`
   - `OSCMessage`

### 2. Code Configuration

1. Open the code in the Arduino IDE.
2. Update the WiFi credentials:
   ```cpp
   const char* ssid = "YourWiFiSSID";
   const char* password = "YourWiFiPassword";
### 3. Upload the Code

1. Connect the TTGO T-Relay board to your computer via USB-C.
2. Select the correct board and port in the Arduino IDE.
3. Upload the code to the ESP32.

### 4. Configure TouchOSC

1. Design a layout in the TouchOSC editor with at least 4 faders.
2. Assign OSC addresses to the faders:
   - `/1/fader1` for Relay 1
   - `/1/fader2` for Relay 2
   - `/1/fader3` for Relay 3
   - `/1/fader4` for Relay 4
3. Set the OSC host to the ESP32's IP address and the port to `8000`.

## Usage

1. Power on the TTGO T-Relay board and connect it to your WiFi network.
2. Open the TouchOSC app and connect to the board using its IP address.
3. Adjust the faders to control the relays:
   - **Fader Value < 0.3**: Relay off.
   - **Fader Value ≥ 0.3**: Relay toggles at intervals determined by the value.

## Example Fader to Relay Mapping

| Fader Address | Relay Pin | Toggle Duration (ms) Based on Fader Value |
|---------------|-----------|-------------------------------------------|
| `/1/fader1`   | GPIO 21   | 300, 200, 100, 50, 40, 10                |
| `/1/fader2`   | GPIO 19   | 300, 200, 100, 50, 40, 10                |
| `/1/fader3`   | GPIO 18   | 300, 200, 100, 50, 40, 10                |
| `/1/fader4`   | GPIO 5    | 300, 200, 100, 50, 40, 10                |
## Troubleshooting

- **No WiFi Connection**: Ensure WiFi credentials are correct and the network is 2.4GHz.
- **TouchOSC Not Responding**: Verify the OSC host IP and port settings in the app.
- **Relays Not Working**: Ensure the relays are connected properly and GPIO pin assignments are correct.
- **Laggy Response**: Reduce network traffic or use a dedicated WiFi network.

## Pinout

| Relay | GPIO Pin |
|-------|----------|
| 1     | 21       |
| 2     | 19       |
| 3     | 18       |
| 4     | 5        |

## Example Code Snippet

```cpp
if (faderValue >= 0.3 && faderValue < 0.4) {
  duration = 300;
} else if (faderValue >= 0.4 && faderValue < 0.5) {
  duration = 200;
} else if (faderValue >= 0.5 && faderValue < 0.6) {
  duration = 100;
} else if (faderValue >= 0.6 && faderValue < 0.7) {
  duration = 50;
} else if (faderValue >= 0.7) {
  duration = 10;
}
