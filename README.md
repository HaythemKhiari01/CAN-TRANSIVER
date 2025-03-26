# CAN Bus DC Motor Control with STM32F407

## Overview
This project implements a **DC motor control system** using the **Controller Area Network (CAN) protocol** on an **STM32F407 microcontroller**. The system utilizes different CAN modes, including **normal mode, loopback mode, and driver mode**, to transmit and receive data efficiently.

## Features
- **CAN Communication:** Implements **normal mode, loopback mode, and driver mode**.
- **Real-Time Data Transmission:** Uses CAN bus to send and receive motor control signals.
- **ADC Integration:** Reads sensor values and transmits them via CAN.
- **Interrupt-Based Message Handling:** Efficiently processes CAN messages using **ISR (Interrupt Service Routines)**.
- **Modular Code Structure:** Separate modules for CAN driver, UART, ADC, and GPIO.

## Project Structure
```
can_dc_motor_control/
├── can_driver/         # CAN driver source files
├── can_normalmode_rx/  # Normal mode receiver source
├── can_normalmode_tx/  # Normal mode transmitter source
├── can_loopback/       # Loopback mode source
```

## Requirements
- **STM32F407 Discovery Board**
- **STM32CubeIDE**
- **GCC ARM Toolchain**
- **CAN Bus Transceiver (e.g., MCP2551, SN65HVD230)**
- **DC Motor and Motor Driver (e.g., L298N, DRV8833)**

## Installation & Compilation
1. Clone this repository:
   ```bash
   git clone https://github.com/HaythemKhiari01/CAN-TRANSIVER.git
   ```
2. Open the project in **STM32CubeIDE**.
3. Build and flash the firmware to the STM32F407 board.

## CAN Modes Implementation
### **Normal Mode (Receiver & Transmitter)**
- **`can_normalmode_rx.c`**: Receives CAN messages and processes motor control commands.
- **`can_normalmode_tx.c`**: Reads sensor values via ADC and transmits data over CAN.

### **Loopback Mode**
- **`can_loopback.c`**: Tests CAN communication by sending and receiving messages within the same microcontroller.

### **CAN Driver**
- **`can_driver.c`**: Implements CAN initialization, message transmission, and reception using interrupts.

## Usage
The application transmits ADC sensor data via CAN and toggles an LED upon message reception:
1. **Receiver (RX Mode)**:
   - Waits for CAN messages.
   - Toggles an LED when a message is received.
2. **Transmitter (TX Mode)**:
   - Reads ADC sensor values.
   - Sends sensor data via CAN.

## Future Improvements
- Implement **bidirectional motor control** using CAN commands.
- Add **error handling and message filtering**.
- Optimize **power efficiency and timing**.

## License
This project is provided AS-IS under the **STMicroelectronics License**.

---
*Developed using STM32CubeIDE & STM32F4 HAL Libraries.*

