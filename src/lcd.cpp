#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>

#include "lcd.h"


LCD::LCD() {
    file = open("/dev/i2c-1", O_RDWR);

    if (file < 0) {
        std::cout << "Erreur lors de l'ouverture de l'I2C" << std::endl;
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, LCD_I2C_ADDR) < 0) {
        std::cout << "Impossible de communiquer avec l'écran LCD" << std::endl;
        exit(1);
    }

    write_byte(LCD_COMMAND_REG, LCD_FUNCTION_SET | LCD_2LINE | LCD_5x10DOTS);
    usleep(45);

    write_byte(LCD_COMMAND_REG, LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
    usleep(45);

    clear();
}

inline void LCD::write_byte(uint8_t reg, uint8_t value) const {
    uint8_t data[2] = {reg, value};
    size_t len = write(file, data, 2);

    if (len != 2) {
        std::cout << "Erreur lors de l'écriture de " << value << " dans " << reg << std::endl;
        exit(1);
    }
}

inline void LCD::clear() {
    write_byte(LCD_COMMAND_REG, LCD_CLEAR);
    usleep(500);
}

inline void LCD::print(const std::string& str) {
    for (char c: str)
        write_byte(LCD_DATA_REG, c);
}
