

#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <mgos.h>
#include <mgos_i2c.h>

#define DEFAULT_LCD_ADDRESS 0x27 //< Default I2C Slave Address

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// pin/port bit
#define RS  1 //B00000001 // P0 : RS bit
#define RW  2 //B00000010 // P1 : R/W bit
#define EN  4 //B00000100 // P2 : Enable bit
#define BL  8 //B00001000 // P3 : BACKLIGHT bit
#define DB4 16 //B00010000 // P4 : DB4 bit
#define DB5 32 //B00100000 // P5 : DB5 bit
#define DB6 64 //B01000000 // P6 : DB6 bit
#define DB7 128 //B10000000 // P7 : DB7 bit


class LiquidCrystal_I2C {
  public:
    LiquidCrystal_I2C(uint8_t addr = DEFAULT_LCD_ADDRESS);
    void init(void);
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

    void clear();
    void home();

    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();

    void setRowOffsets(int row1, int row2, int row3, int row4);
    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t);

    void write(uint8_t);
    void command(uint8_t);

    void print(const char[]);
    void print(long n);
    void print(double n, int digits);

private:
    void printFloat(double number, uint8_t digits);
    void printNumber(unsigned long n, uint8_t base);
    void write(const uint8_t *buffer, uint8_t size);
    void write(const char *str);

    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void pulseEnable(uint8_t);
    void writeI2c(uint8_t);

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _numlines;
    uint8_t _row_offsets[4];

    uint8_t _i2caddr;
    uint8_t _backlight;

    struct mgos_i2c *myi2c;
};

#endif
