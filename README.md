# STM32F4 CAN Transceiver Project

## Project Overview
This project demonstrates CAN (Controller Area Network) communication using an STM32F4 microcontroller in different operational modes, including Normal Mode and Loopback Mode.

## Project Structure
The project contains four main source files:
- `can_normal_mode_rx.c`: Normal mode receiver implementation
- `can_normal_mode_tx.c`: Normal mode transmitter implementation
- `can_driver.c`: CAN driver source file
- `can_loopback.c`: Loopback mode implementation for internal testing

## Key Features
- CAN communication in Normal and Loopback modes
- ADC (Analog-to-Digital Converter) integration
- Interrupt-driven CAN message reception
- Configurable CAN parameters

## Hardware Requirements
- STM32F4 microcontroller
- CAN transceiver
- ADC-capable pin (PA1 used in this implementation)
- LED for visual feedback

## Software Components
- FPU (Floating-Point Unit) initialization
- Timebase initialization
- GPIO configuration
- CAN communication setup
- ADC reading and conversion

## CAN Configuration
- Standard ID mode
- Configurable CAN ID (0x244 and 0x544 used in examples)
- Data Length Code (DLC) of 2 bytes
- Interrupt-based message reception

## Main Functionality
1. Initialize microcontroller peripherals
2. Configure CAN communication
3. Set up ADC reading
4. Transmit ADC values over CAN
5. Handle received CAN messages via interrupt

## Modes of Operation
### Normal Mode
- Actual network communication
- Transmits ADC readings
- Receives CAN messages

### Loopback Mode
- Internal testing mode
- Sends predefined messages to itself
- Verifies CAN communication without external network

## Interrupt Handling
- `CAN1_RX0_IRQHandler()` manages incoming CAN messages
- Increments a message count
- Toggles LED (in receive mode)

## Compilation Notes
- Requires STM32F4xx HAL libraries
- Depends on custom headers: 
  - `can_driver.h`
  - `uart.h`
  - `bsp.h`
  - `fpu.h`
  - `adc.h`
  - `timebase.h`

## Potential Improvements
- Add error handling
- Implement more sophisticated message processing
- Add configuration for different CAN bit rates
- Implement bidirectional communication

## Troubleshooting
- Ensure proper CAN transceiver connections
- Verify interrupt configuration
- Check ADC initialization
- Validate CAN filter settings

