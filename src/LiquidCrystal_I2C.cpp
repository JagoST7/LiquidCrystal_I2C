#include "LiquidCrystal_I2C.h"
#include <math.h>

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t addr) {
    myi2c = mgos_i2c_get_global();
    _i2caddr = addr;
    _backlight = BL;


    init();
}

void LiquidCrystal_I2C::init() {
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    if (lines > 1) {
    	_displayfunction |= LCD_2LINE;
    }
    _numlines = lines;
    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    if ((dotsize != 0) && (lines == 1)) {
      	_displayfunction |= LCD_5x10DOTS;
    }
    mgos_msleep(50);
    writeI2c(0x00);
    write4bits(DB4|DB5);
    mgos_msleep(5);
    write4bits(DB4|DB5);
    mgos_msleep(5);
    write4bits(DB4|DB5);
    mgos_msleep(1);
    write4bits(DB5);

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);

    home();
}

void LiquidCrystal_I2C::setRowOffsets(int row0, int row1, int row2, int row3) {
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

void LiquidCrystal_I2C::clear(){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	mgos_msleep(2);  // this command takes a long time!
}

void LiquidCrystal_I2C::home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	mgos_msleep(2);  // this command takes a long time!
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row){
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

/**
 @brief Turn the display off (quickly)
*/
void LiquidCrystal_I2C::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief Turn the display on (quickly)
*/
void LiquidCrystal_I2C::display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief Turns the underline cursor off
*/
void LiquidCrystal_I2C::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief Turns the underline cursor on
*/
void LiquidCrystal_I2C::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief Turn off the blinking cursor
*/
void LiquidCrystal_I2C::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief Turn on the blinking cursor
*/
void LiquidCrystal_I2C::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief These commands scroll the display without changing the RAM
*/
void LiquidCrystal_I2C::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/**
 @brief These commands scroll the display without changing the RAM
*/
void LiquidCrystal_I2C::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/**
 @brief This is for text that flows Left to Right
*/
void LiquidCrystal_I2C::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief This is for text that flows Right to Left
*/
void LiquidCrystal_I2C::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief This will 'right justify' text from the cursor
*/
void LiquidCrystal_I2C::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief This will 'left justify' text from the cursor
*/
void LiquidCrystal_I2C::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief Allows us to fill the first 8 CGRAM locations with custom characters
*/
void LiquidCrystal_I2C::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

void LiquidCrystal_I2C::print(const char str[]) {
    write(str);
}

void LiquidCrystal_I2C::print(long n) {
    if(n < 0) {
        print("-");
        printNumber(-n, 10);
    } else {
        printNumber(n, 10);
    }
}

void LiquidCrystal_I2C::print(double n, int digits) {
    return printFloat(n, digits);
}
//

void LiquidCrystal_I2C::printNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if(base < 2) {
        base = 10;
    }

    do {
        unsigned long m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);

    write(str);
}

void LiquidCrystal_I2C::printFloat(double number, uint8_t digits)
{
    if(isnan(number)) {
         print("nan");
         return;
    }
    if(isinf(number)) {
        print("inf");
        return;
    }
    if(number > 4294967040.0) {
        print("ovf");    // constant determined empirically
        return;
    }
    if(number < -4294967040.0) {
        print("ovf");    // constant determined empirically
        return;
    }

    // Handle negative numbers
    if(number < 0.0) {
        print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for(uint8_t i = 0; i < digits; ++i) {
        rounding /= 10.0;
    }

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = number - (double) int_part;
    print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if(digits > 0) {
        print(".");
    }

    // Extract digits from the remainder one at a time
    while(digits-- > 0) {
        remainder *= 10.0;
        int toPrint = int(remainder);
        print(toPrint);
        remainder -= toPrint;
    }

}

void LiquidCrystal_I2C::write(const char *str) {
    write((const uint8_t *)str, strlen(str));
}

void LiquidCrystal_I2C::write(const uint8_t *buffer, uint8_t size) {
    while(size--) {
        write(*buffer++);
    }
}

void LiquidCrystal_I2C::write(uint8_t value) {
  send(value, RS);
}

void LiquidCrystal_I2C::command(uint8_t value) {
  send(value, 0);
}

//private

void LiquidCrystal_I2C::writeI2c(uint8_t data) {
    data = data | 8;
    mgos_i2c_write(myi2c, _i2caddr, &data , 1 , true);
}

void LiquidCrystal_I2C::pulseEnable(uint8_t value) {
  writeI2c(value & ~EN); // EN LOW
  mgos_usleep(1);
  writeI2c(value | EN);  // EN HIGH
  mgos_usleep(1);  // enable pulse must be >450ns
  writeI2c(value & ~EN); // EN LOW
  mgos_usleep(50); // commands need > 37us to settle
}

void LiquidCrystal_I2C::write4bits(uint8_t value) {
  writeI2c(value);
  pulseEnable(value);
}

void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
  uint8_t Hbit = value & 0xF0;
  uint8_t Lbit = (value << 4) & 0xF0;
  write4bits(Hbit | mode);
  write4bits(Lbit | mode);
}



