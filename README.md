# Arduino Ping Pong (Puck Game)

Two-player reaction-based electronic puck game built using Arduino Nano.

## Features
- Joystick-controlled gameplay
- Real-time score tracking with 7-segment displays
- Shift-register-based display control
- Audio feedback via buzzer
- Custom-designed PCB

## Repository Structure
- Software/ – Arduino source code
- PONG/ – Schematics, PCB layouts
- PONG_GERB/ - Drill, Gerber Files
- Datasheets/ - Parts used



## PCB Build & Assembly Instructions

This repository contains all files required to manufacture, assemble, and program the Arduino Ping Pong PCB.

### 1. PCB Manufacturing
Fabricate the PCB using the Gerber and drill files located in:
- `PONG/`
- `PONG_GERB/`

Upload these files to a standard PCB manufacturer (e.g., JLCPCB, PCBWay) using typical default settings:
- Board thickness: 1.6 mm
- Copper weight: 1 oz
- Surface finish: HASL or ENIG
- Solder mask: Any color

### 2. Required Components
- Arduino Nano
- 74HC595 shift registers
- Two joystick modules
- Two 7-segment displays
- tow Buzzer
- 330ohm Resistors (current-limiting and pull-ups as specified in schematic)
- 1 uf Capacitors (decoupling)
- Female connectors, normal and 90 degree configuration
- Solder station

### 3. Required Softwareew
- Arduino ide
- Kicad
  

Refer to the schematic for exact component values and connections:
- `PONG/`

### 4. PCB Assembly
1. Begin by soldering low-profile components (resistors and capacitors).
2. Solder ICs (74HC595 shift registers) and header pins.
3. Mount and solder the Arduino Nano.
4. Install the 7-segment displays, joystick connectors, and buzzer.
5. Visually inspect all solder joints for shorts, cold joints, or misalignment.

### 5. Firmware Upload
1. Connect the Arduino Nano to a computer via USB.
2. Open the Arduino IDE.
3. Load the firmware located in:
   - `Software/`
4. Select the correct board (Arduino Nano) and COM port.
5. Upload the firmware to the board.

### 6. Initial Power-On and Testing
- Power the board using USB (5 V).
- Confirm that the 7-segment displays initialize correctly.
- Verify joystick inputs respond to movement.
- Check that the buzzer activates during scoring events.

### 7. Gameplay Verification
- Ensure both players can control their Joysticks.
- Confirm scores increment correctly.
- Verify game reset and restart behavior.

Once all checks pass, the PCB is ready for normal gameplay, HAVE FUN!!


