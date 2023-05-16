//
// Created by mrspaar on 5/16/23.
//

#ifndef GRAPH_LCD_H
#define GRAPH_LCD_H

#include <cstdint>
#include <string>


#define LCD_I2C_ADDR         0x3E
#define LCD_DATA_REG         0x40
#define LCD_COMMAND_REG      0x80

#define LCD_FUNCTION_SET     0x20
#define LCD_2LINE            0x08
#define LCD_5x10DOTS         0x04

#define LCD_DISPLAY_CONTROL  0x08
#define LCD_DISPLAY_ON       0x04
#define LCD_DISPLAY_OFF      0x00

#define LCD_CLEAR            0x01


class LCD {
public:
    LCD();

    void clear();
    void print(const std::string& str);
    void set_cursor(uint8_t row, uint8_t col);
private:
    int file;
    void write_byte(uint8_t reg, uint8_t value) const;
};


#endif //GRAPH_LCD_H
