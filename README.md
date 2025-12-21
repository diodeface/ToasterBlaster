
<p align="middle"> 
    <img src="docs/images/splash_big.png" height="300" />
</p>

<p float="left" align="middle">
    <img src="docs/images/preview.gif" height="150" />
    <img src="docs/images/hud_1.jpg" height="150" />
    <img src="docs/images/hud_2.jpg" height="150" />
</p>

<p align="middle"> 
This is an LED control program for protogen helmets to be used with MAX7219 LED matrixes and ESP32 dev boards.<br>
Note that this is a pre-release with some bugs, unfinished code and a major lack of documentation.<br>
Use at your own risk.
</p>

# Setup
*For now I can provide a brief setup guide as I don't have time to write detailed documentation. Sorry!*<br><br>

## Hardware

The following hardware is supported, with more to come in the future:
- ESP32-WROOM dev board
- MAX7219 LED Dot matrix modules (Two 32x8 (FC16) modules for mouth, one 32x8 cut in half for eyes and two 8x8 modules for nose)
- SSD1306 128x64 OLED I2C display (Heads up display)
- MOCUTE 052 Bluetooth Gamepad
- TCRT5000 Infrared obstacle avoidance sensor (Boop sensor, analog and digital)
- WS2812 LED strip or something similar supported by the FastLED library
- MAX9814 Electret microphone module
- Any 5V PWM controlled fan (Noctua NF-A4x20 5V PWM)

Most if not all hardware can be purchased on sites like aliexpress or ebay. <br><br>

## Wiring

### MAX7219 LED Dot matrix
MAX7219 can be daisy chained. Wire up displays in this order:<br>

- ESP32
1. Eye Left (16x8)
2. Mouth Left (32x8)
3. Nose Left (8x8)
4. Nose Right (8x8)
5. Mouth Right (32x8)
6. Eye Right (16x8)

<br>

| MAX7219 | ESP32 |
| - | - |
| DIN | 23 |
| CS | 5 |
| CLK | 18 |

**Use an external 5V power supply!** Powering the matrixes from the 5V pin on your ESP32 board is not recommended. Also avoid using dupont cables and solder directly instead.<br><br>

### SSD1306 heads up display
| SSD1306 | ESP32 |
| - | - |
| SCL | 22 |
| SDA | 21 |

Both 3.3V or 5V should work for power<br><br>

### TCRT5000 infrared sensor
**For better tracking and easier calibration use the analog output.**
| TCRT5000 | ESP32 |
| - | - |
| A0 | 39 (marked SN or VN) for analog |
| D0 | 19 for digital |

Use 3.3V for power<br><br>

### WS2812 LED strip
| WS2812 | ESP32 |
| - | - |
| DIN | 15 |

Use an **external** 5V power supply<br><br>

### MAX9814 Electret microphone module
| MAX9814 | ESP32 |
| - | - |
| DIN | 36 (marked VP) |

Connect VDD to 3.3V<br>
Connect AR to GND for a 1:500 attack/release ratio<br>
Connect GAIN to VDD for a 40dB gain<br>

### Fan PWM Control
| Fan header | ESP32 |
| - | - |
| PWM (Blue) | 16 |

Use an **external** 5V power supply<br><br>

## Software setup
- You'll need [Visual Studio code](https://code.visualstudio.com) with the PlafromIO IDE extension installed. 
- Clone this project and add it in the PlatformIO menu (**PlatformIO -> PIO Home -> Projects & Configuration -> Add Existing**). This should set up a development environment and download all necessary dependencies.
- Press **PlatformIO: Build (Ctrl+Alt+B)** to build your project.
- When ready, connect your ESP32 board and press **PlatformIO: Upload (Ctrl+Alt+U)**. Wait until *Connecting...* message shows up and hold down the **BOOT** button on your ESP32 until it starts uploading.
- Turn on your gamepad and flip **KEY/GAME** switch to **GAME** position.
- Enjoy!

## Testing without hardware
- Included are diagram.json and wokwi.toml files for testing using the [Wokwi Simulator](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) plugin for VS Code.
- Install the plugin according to the [Wokwi documentation](https://docs.wokwi.com/vscode/getting-started#installation).
- Build the program first with PlatformIO **(Ctrl+Alt+B)**. Then, press **F1** and select **Wokwi: Start Simulator**
- If nothing happens for a while, try disabling the gamepad in config.h and try again.
- Note that the simulation is going to be considerably slower than hardware. I only recommend using this approach for quickly testing new ideas without hardware.

## Default controller mapping
The default button mapping in "GAME" mode is as follows:
| Buttons | Action |
| - | - |
| BACK, MENU, X | Change face set (FaceSwitcherController) |
| Joystick movement | Current controller action (Change face, navigate menu) |
| Trigger | Blink eye |
| Y | Run automatic face switcher (AutomaticFaceSwitcherController) |
| B | Show Stats/Settings (SettingsController) |
| A | Calibrate analog boop sensor |

"KEY" mode is currently unsupported.

## Editing face bitmaps:
- Check the [Bitmaps directory](src/Assets/Bitmaps) containing images for eyes, mouth and nose. 
- Images are converted from png to source code during the build process. Simply edit the images and rebuild (Ctrl+Alt+B).

## (todo) Quick configuration
- Important configuration constants can be found in [config.h](src/config.h)
- `setup()` function in [main.cpp](src/main.cpp) handles registering and initializing displays, peripherals, used transitions and overlays
- Change button mapping in the `createButtonMapping()` function
- Change settings menu in the `createSettingsMenu()` function
- Debug messages DDEBUG, DBTDEBUG and DMICDEBUG can be enabled in [platformio.ini](platformio.ini)
<br><br>

## (todo) Display configuration
<br><br>

## (todo) Custom animations
- [Sequences.h](src/Assets/Sequences/Standard.h), [Sequences.cpp](src/Assets/Sequences/Standard.cpp) store animation sequences. Read the included [README](src/Assets/Sequences/README.txt) file.
- [Transitions.h](src/Assets/Transitions/Standard.h), [Transitions.cpp](src/Assets/Transitions/Standard.cpp), [Overlays.h](src/Assets/Overlays/Standard.h), [Overlays.cpp](src/Animation/Overlay/Standard.cpp) for overlay sequences and transitions.
<br><br>

# (todo) Contributing
<br><br>

# Credits
- [olikraus/u8g2](https://github.com/olikraus/u8g2)
- [FastLED/FastLED](https://github.com/FastLED/FastLED)
- [kosme/arduinoFFT](https://github.com/kosme/arduinoFFT)
- [MikeLankamp/fpm](https://github.com/MikeLankamp/fpm)
- [h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)

# License
This program is licensed under [GNU Affero General Public License v3.0](LICENSE).