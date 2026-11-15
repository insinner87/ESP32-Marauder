# RPi 3.5" LCD V3.0 → ESP32 Wiring (ESP32Marauder)

This build runs ESP32Marauder on a generic **ESP32 Dev Module** wired to a
**Raspberry Pi 3.5" LCD V3.0** (ILI9486 driver, XPT2046 resistive touch).

## Final verified wiring

| LCD physical pin | LCD signal       | ESP32 GPIO |
| ---------------- | ---------------- | ---------- |
| 1                | LED / Backlight  | 3.3V       |
| 2                | VCC              | 5V         |
| 6                | GND              | GND        |
| 18               | LCD_RS / DC      | **GPIO 16** |
| 19               | MOSI / T_DIN     | **GPIO 23** |
| 21               | MISO / T_DO      | **GPIO 19** |
| 22               | T_IRQ            | (not connected) |
| 23               | SCK / T_CLK      | **GPIO 18** |
| 24               | LCD_CS           | **GPIO 17** |
| 26               | T_CS             | **GPIO 21** |
| —                | LCD_RST          | **GPIO 5**  |

SPI bus is shared between display and touch (standard VSPI pins).
Backlight is tied directly to 3.3V (no PWM control).

## Textual pinout diagram

```
        +-----------------+                  +--------------+
        | RPi 3.5" LCD    |                  |  ESP32 Dev   |
        |     V3.0        |                  |    Module    |
        +-----------------+                  +--------------+
  BL -> | 1  LED          |------ 3.3V ----> | 3V3          |
 VCC -> | 2  VCC          |------ 5V   ----> | VIN / 5V     |
 GND -> | 6  GND          |------ GND  ----> | GND          |
  DC -> | 18 LCD_RS       |----------------- | GPIO 16      |
MOSI->  | 19 MOSI/T_DIN   |----------------- | GPIO 23      |
MISO->  | 21 MISO/T_DO    |----------------- | GPIO 19      |
 SCK -> | 23 SCK/T_CLK    |----------------- | GPIO 18      |
LCS ->  | 24 LCD_CS       |----------------- | GPIO 17      |
TCS ->  | 26 T_CS         |----------------- | GPIO 21      |
RST ->  |    LCD_RST      |----------------- | GPIO  5      |
        +-----------------+                  +--------------+
```

## ESP32 GPIOs in use

| GPIO | Purpose              |
| ---- | -------------------- |
|  5   | LCD_RST              |
| 16   | LCD_DC               |
| 17   | LCD_CS               |
| 18   | SPI SCK (shared)     |
| 19   | SPI MISO (shared)    |
| 21   | Touch CS             |
| 23   | SPI MOSI (shared)    |

## Firmware-side GPIOs (remapped away from display pins)

Marauder's default `MARAUDER_CYD_3_5_INCH` profile was designed for the
all-in-one CYD board, which uses several of the pins this RPi panel occupies.
These were remapped in `esp32_marauder/configs.h`:

| Function          | CYD default | RPi build |
| ----------------- | ----------- | --------- |
| SD chip-select    | GPIO 5      | GPIO 4    |
| Flipper LED Blue  | GPIO 17     | GPIO 14   |
| Flipper LED Green | GPIO 16     | GPIO 13   |
| Flipper LED Red   | GPIO 22     | GPIO 22   |
| GPS TX            | GPIO 21     | GPIO 26   |
| GPS RX            | GPIO 25     | GPIO 25   |
| TFT_BL (dummy)    | undefined   | GPIO 27   |

## TFT_eSPI library configuration

`third_party_libraries/TFT_eSPI/User_Setup.h` is the exact library this build
was compiled against. Key settings:

```c
#define RPI_DISPLAY_TYPE
#define RPI_ILI9486_DRIVER
#define ILI9486_DRIVER

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  5
#define TOUCH_CS 21

#define SPI_FREQUENCY       20000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
```

## Arduino IDE settings

- **Board:** ESP32 Dev Module
- **Partition Scheme:** Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)
  (or Huge APP (3MB No OTA/1MB SPIFFS) if Minimal is still too small)
- **Flash Size:** 4MB (32Mb) minimum
- **PSRAM:** Disabled (unless your module is a WROVER)
- **Upload Speed:** 921600
- **CPU Frequency:** 240 MHz

## Touch calibration

Values captured with `TFT_eSPI`'s `calibrateTouch()` on this exact panel
(already baked into `esp32_marauder/Display.cpp`):

- Portrait  (rotation 0): `{ 292, 3518, 239, 3619, 7 }`
- Landscape (rotation 1): `{ 214, 3662, 297, 3513, 2 }`

## Build & flash

1. Install **Arduino IDE 2.x** with the **ESP32 board package** (Espressif
   Systems, 2.x or 3.x both work).
2. Copy `third_party_libraries/TFT_eSPI` and
   `third_party_libraries/XPT2046_Touchscreen` into your Arduino libraries
   folder (`~/Documents/Arduino/libraries/`), replacing any existing copy.
3. Open `esp32_marauder/esp32_marauder.ino` in Arduino IDE.
4. Apply the Arduino IDE settings listed above.
5. Upload.
