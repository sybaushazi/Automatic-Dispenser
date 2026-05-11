# Automatic Water Dispenser

## Description
The Automatic Water Dispenser is a smart, sensor-based system built using 
an Arduino UNO that automates the process of dispensing precise amounts 
of water. The system is designed to be intuitive and user-friendly — 
simply select the desired water amount from the menu, place your glass 
in front of the sensor, and the machine does the rest. It eliminates the 
need for manual measurement and ensures accurate, hygienic water dispensing 
every time.

---

## How It Works
1. **Menu Selection** — An OLED display shows a menu with 3 options:
   - 100 ml
   - 150 ml
   - 200 ml
   
   A button is used to navigate through the menu and select the desired amount.

2. **Glass Detection** — Once an amount is selected, the Ultrasonic Sensor 
detects when a glass is placed in front of the dispenser. The water pump 
is then automatically triggered and dispenses exactly the selected amount 
of water.

3. **Water Level Monitoring** — A Water Level Sensor continuously monitors 
the water level in the reservoir. When the water is running low, the OLED 
displays a **"Water Low"** warning message, the Active Buzzer beeps as an 
alert, and the LED lights up to give a visual warning.

---

## Components Used
| Component | Purpose |
|---|---|
| Arduino UNO | Main microcontroller |
| OLED Display | Shows menu and water level warnings |
| Ultrasonic Sensor | Detects glass placement |
| Water Level Sensor | Monitors water level in reservoir |
| Mini Submersible Water Pump | Pumps and dispenses water |
| Single Channel Relay | Controls the water pump |
| Active Buzzer | Beeps when water is low |
| LED | Visual indicator for low water |
| Resistor | Current limiting for LED |
| Capacitor | Stabilizes power supply |

---

## Project Pictures
Check out the `Project Pictures` folder in this repo to see our build!

---

## Code
The Arduino code is available in the file `Final_arduino code.ino` in this repo.

---

## Proposal
The full project proposal is available in this repo as a PDF.

---

## Team Members
| Name | Roll Number |
|------|-------------|
| Shahzaib Sohail | 25i-6649 |
| Mateen Ahmed | 25i-6614 |
