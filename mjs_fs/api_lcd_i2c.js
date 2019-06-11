let LCD_I2C = {
    _init: ffi('void *mgos_lcd_i2c_create(int)'),
    _dlt: ffi('void mgos_lcd_i2c_delete(void *)'),
    _bgn: ffi('void mgos_lcd_i2c_begin(void *, int, int, int)'),
    _clr: ffi('void mgos_lcd_i2c_clear(void *)'),
    _hom: ffi('void mgos_lcd_i2c_home(void *)'),
    _stcrs: ffi('void mgos_lcd_i2c_setCursor(void *, int, int)'),
    _prntchar: ffi('void mgos_lcd_i2c_print(void *, char *)'),
    _prntdouble: ffi('void mgos_lcd_i2c_printDouble(void *, double, int)'),

    DEFAULT_LCD_ADDRESS: 0x27,
    LCD_5x10DOTS: 0x04,
    LCD_5x8DOTS: 0x00,

    create: function (addr) {
        let obj = Object.create(LCD_I2C._proto);
        if (!addr) {
            addr = LCD_I2C.DEFAULT_LCD_ADDRESS;
        }
        obj.lcd = LCD_I2C._init(addr);
        return obj;
    },

    _proto: {
        close: function () {
            return LCD_I2C._dlt(this.lcd);
        },
        begin: function (row, col, charsize) {
            if (!charsize) {
                charsize = LCD_I2C.LCD_5x8DOTS;
            }
            return LCD_I2C._bgn(this.lcd, row, col, charsize);
        },
        clear: function () {
            return LCD_I2C._clr(this.lcd);
        },
        home: function () {
            return LCD_I2C._hom(this.lcd);
        },
        setCursor: function (row, col) {
            return LCD_I2C._stcrs(this.lcd, row, col);
        },
        print: function (n, digits) {
            if (digits) {
                return LCD_I2C._prntdouble(this.lcd, n, digits);
            }
            if (typeof (n) === "number") {
                return LCD_I2C._prntchar(this.lcd, JSON.stringify(n));
            }
            return LCD_I2C._prntchar(this.lcd, n);
        },
    },
};