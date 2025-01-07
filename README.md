# Arduino GPS and Motion Tracking System

This project implements a tracking system using Arduino, combining GPS location tracking (NEO-6M-V2) and motion sensing (MPU6050) capabilities.

## Components Required

- Arduino Board (UNO/Nano/etc.)
- MPU6050 (Accelerometer and Gyroscope module)
- NEO-6M-V2 GPS Module
- Connecting wires
- Power supply

## Features

- Real-time GPS location tracking
- Motion detection and orientation sensing
- Accelerometer and gyroscope data processing
- Serial communication for data output

## Pin Connections

### MPU6050 Connection
- VCC → 5V
- GND → GND
- SCL → A5
- SDA → A4

### NEO-6M-V2 Connection
- VCC → 5V
- GND → GND
- TX → Digital Pin 4
- RX → Digital Pin 3

## Installation

1. Install the required Arduino libraries:
   - TinyGPS++ library for GPS module
   - Wire library for I2C communication
   - MPU6050 library for motion sensing

2. Connect the components according to the pin connection guide above

3. Upload the code to your Arduino board

## Usage

1. Power up the Arduino board
2. Wait for GPS signal acquisition (the GPS LED will blink when searching for satellites)
3. The system will start tracking location and motion data
4. Monitor the data through the Serial Monitor at 9600 baud rate

## Data Output

The system provides the following data:
- GPS coordinates (latitude and longitude)
- Altitude
- Speed
- Acceleration in X, Y, Z axes
- Gyroscope readings
- Temperature

## Contributing

Feel free to contribute to this project by submitting issues or pull requests.

## License

This project is open source and available under the MIT License. 