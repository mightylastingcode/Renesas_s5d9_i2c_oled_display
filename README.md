# Renesas_s5d9_i2c_oled_display
The project uses C code to read the on board temperature sensor and show the reading on an OLED display module.

Develop the C code from scratch for the S5D9 board
Read the current temperature (F) from onboard AMS EN210 sensor (I2C interface).
Display the current temperature (F) on OLED display (I2C interface).
Display the temperature graphically over time on OLED display.

File Description:
1.  hal_entry.c/h  (Main Program Loop)
2.  i2c.c/h (i2c SSP API calls)
3.  en210.c/h (subroutines for AMS EN210 operations)
4.  ssd1306_oled.c/h (subroutines for OLED module operations)

Datasheet:
ENS210_DS000470_1-00.pdf (Temperature sensor datasheet)
OLED Sepecification.pdf (OLED display module spec sheet)
SSD1306.pdf (OLED display driver chip datasheet)
an amazing projects

https://www.miketechuniverse.com/single-post/2017/10/27/Tutorial-for-Renesas-S5D9-board-Show-The-Temperature-Reading-on-an-OLED-Display
