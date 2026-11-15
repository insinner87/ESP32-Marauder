// ESP32 + 3.5" RPi LCD V3.0 (ILI9486 + XPT2046)

#define USER_SETUP_ID 11

#define RPI_DISPLAY_TYPE
#define ILI9486_DRIVER

// Display SPI pins
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18

// Display control pins
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  5

// Touch controller
#define TOUCH_CS 21

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

// SPI speeds
#define SPI_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000