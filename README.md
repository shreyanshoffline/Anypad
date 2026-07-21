# The Anypad ♾️

A custom left-hand gaming macropad that *actually* respects your muscle memory. Built for the Hack Club Hackpad program.

Most macropads use a rigid ortholinear grid, which is terrible for gaming. If you try to play Minecraft, Fortnite, or Roblox on a square grid, your fingers get confused because normal keyboards have a staggered layout. 

**The Anypad** fixes this. It features a true QWERTY-accurate 0.25U stagger for the WASD movement cluster, a dedicated thumb spacebar, and stacked pinky keys for Shift/Ctrl. Add an OLED screen, a rotary encoder, and an aggressive amount of RGB, and you have the ultimate mini-pad designed to seamlessly swap between gaming, office, and study modes.

## 📸 The Screenshots

These are some screenshots of the PCB, Schematic, Case, And Complete Build.

### Overall Hackpad
![Overall Hackpad](./assets/overall.png)

*(Note: Coming soon! After Build)*

### Schematic
![Schematic](./assets/Schematic.png)

### PCB Layout
![PCB Layout](./assets/PCB.png)
*(Check out the custom silkscreen art on the right!)*

### CAD in Fusion360
![Case Assembly](./assets/CAD.png)

---

## ✨ Features
* **Muscle-Memory Layout:** 9 mechanical switches arranged with realistic desktop stagger so you don't miss inputs mid-game or while locked in during work.
* **The Brain:** Powered by The Seeed Studio XIAO RP2040 Microcontroller. Small, But Mighty
* **The Screen:** A 0.91" OLED display to give visual feedback on your current active profile (Gaming 🎮 / Office 🏢 / Study ✏️).
* **The Knob** Instead of using keys to switch inventory slots, or raise and reduce volume the knob allows you to do more while taking less space.

## ✅ Benefits
* **Light, Small, Fast:** Instead of carrying an entire keyboard all you need is a mouse and The Anypad. Easily portable and gives you more desk space.
* **Focus, Study, Game:** Switch between focus modes in the swift turn of a knob. Quickly immerse your self in the gaming world or into optimized work and study shortcuts
* **RGB Everything:** 17 total SK6812 MINI-E addressable LEDs (9 per-key backlights + 8 corner underglows for a seamless desk halo effect). Instantly turns up your vibe
* **Ergonomics Over Symmetry:** The Anypad prioritizes hand comfort over artificial symmetry. While the layout might look like an unusual angle at first glance, it naturally aligns with where your fingers want to rest during long gaming sessions.

## </> Firmware Overview
The Anypad uses QMK for everything

Via will be added soon for instant flashing and customization


## 🧰 Bill of Materials (BOM)

| Component | Qty | Description |
| :--- | :---: | :--- |
| Seeed XIAO RP2040 | 1 | The Brains of the system, A Mighty little Microcontroller |
| EC11 Rotary Encoder | 1 | Rotating knob, with push-button functionality |
| Encoder Knob | 1 | Fits over the EC11 shaft |
| 0.91" OLED Display | 1 | Standard 4-pin breakout (GND, VCC, SCL, SDA) |
| Reverse Mount SK6812 MINI-E LEDs | 17 | 9 for Keys, 8 for bottom underglow |
| Through-Hole 1N4148 Diodes | 9 | For the 3x3 anti-ghosting matrix |
| Custom PCB (In /PCB)| 1 | Holds and connects all electrical components |
| Custom 3D Printed Case (In /Production)| 1 | Top plate + bottom enclosure |
| Blank White DSA Keycaps | 9 | 1U caps, Smooth and blank |
| MX-Style Keyswitches | 9 | The Nice Clicky Kind |
| M3x16mm screws | 4 | Tiny Screws That hold everything together |
| M3x5mx4mm Brass Heatset Inserts | 4 | Brass treads that makes the case re-openable |

You may also need a solder, soldering iron and other soldering accessories for completing the build

## 🧠 Behind the Hardware (The Wiring Matrix)

To make everything fit on the XIAO's extremely limited edge pins, the 9 keys are wired into a standard 3x3 diode matrix (using only 6 pins). 

This is a basic efficient layout that frees up the remaining pins perfectly for the OLED (2 pins), the Rotary Encoder (2 pins), and a single data line chained through all 17 LEDs without needing to resort to hidden pads or larger microcontrollers. 

---
*Designed by [shreyanshoffline](https://github.com/shreyanshoffline) for The Stardance Challenge by Hack Club, NASA, AMD, and Github.*
