#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C *mgos_lcd_i2c_create(uint8_t addr = DEFAULT_LCD_ADDRESS);

void mgos_lcd_i2c_delete(LiquidCrystal_I2C *lcd);

void mgos_lcd_i2c_begin(LiquidCrystal_I2C *lcd, uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

void mgos_lcd_i2c_setCursor(LiquidCrystal_I2C *lcd, uint8_t, uint8_t);

void mgos_lcd_i2c_print(LiquidCrystal_I2C *lcd, const char[]);