#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "lcd.h"


LCD::LCD() {
    file = open("/dev/i2c-1", O_RDWR);

    if (file < 0) {
        printf("Erreur lors de l'ouverture de l'I2C\n");
        exit(1);
    }

    int status = ioctl(file, I2C_SLAVE, LCD_I2C_ADDR);

    if (status < 0) {
        printf("Impossible de communiquer avec l'écran LCD (erreur %d)\n", status);
        exit(1);
    }

    write_byte(LCD_COMMAND_REG, LCD_FUNCTION_SET | LCD_2LINE | LCD_5x10DOTS);
    usleep(45);

    write_byte(LCD_COMMAND_REG, LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
    usleep(45);
}

inline void LCD::write_byte(uint8_t reg, uint8_t value) const {
    uint8_t data[2] = {reg, value};
    size_t len = write(file, data, 2);

    if (len != 2) {
        printf("Erreur (%zu) lors de l'écriture de % dans %d\n", len, value, reg);
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
