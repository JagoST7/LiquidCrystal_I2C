#include "mgos_lcd_i2c.h"

LiquidCrystal_I2C *mgos_lcd_i2c_create(uint8_t addr) {
    return new LiquidCrystal_I2C(addr);
}

void mgos_lcd_i2c_delete(LiquidCrystal_I2C *lcd) {
    if (lcd != nullptr) {
        delete lcd;
        lcd = nullptr;
    }
}

void mgos_lcd_i2c_begin(LiquidCrystal_I2C *lcd, uint8_t cols, uint8_t rows, uint8_t charsize) {
    if (lcd != nullptr) {
        lcd->begin(cols, rows, charsize);
    }
}

void mgos_lcd_i2c_setCursor(LiquidCrystal_I2C *lcd, uint8_t col, uint8_t row){
    if (lcd != nullptr) {
        lcd->setCursor(col, row);
    }
}

void mgos_lcd_i2c_print(LiquidCrystal_I2C *lcd, const char str[]){
    if (lcd != nullptr) {
        lcd->print(str);
    }
}