#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>

#include "hardware/lcd.h"


int LCD::init() {
    file = open("/dev/i2c-1", O_RDWR);

    if (file < 0)
        return 1;

    if (ioctl(file, I2C_SLAVE, LCD_I2C_ADDR) < 0)
        return 1;

    write_byte(LCD_COMMAND_REG, LCD_FUNCTION_SET | LCD_2LINE | LCD_5x10DOTS);
    usleep(45);

    write_byte(LCD_COMMAND_REG, LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
    usleep(45);

    clear();
    return 0;
}

void LCD::write_byte(uint8_t reg, uint8_t value) const {
    uint8_t data[2] = {reg, value};
    size_t len = write(file, data, 2);

    if (len != 2) {
        std::cout << "Erreur lors de l'écriture de " << value << " dans " << reg << std::endl;
        exit(1);
    }
}

void LCD::clear() {
    write_byte(LCD_COMMAND_REG, LCD_CLEAR);
    usleep(500);
}

void LCD::set_cursor(uint8_t row, uint8_t col) {
    if (row > 1 || col > 15) {
        std::cout << "Erreur: les coordonnées du curseur sont invalides (grille 2x16)" << std::endl;
        exit(1);
    }

    col = (row == 0 ? col | 0x80 : col | 0xC0);
    write_byte(LCD_COMMAND_REG, col);
}

void LCD::print(const std::string& str) {
    if (str.size() > 16) {
        std::cout << "Erreur: la chaîne de caractères est trop longue" << std::endl;
        exit(1);
    }

    for (char c: str)
        write_byte(LCD_DATA_REG, c);
}
