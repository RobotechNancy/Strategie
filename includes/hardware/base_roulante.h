//
// Created by mrspaar on 5/18/23.
//

#ifndef STRATEGIE_BASE_ROULANTE_H
#define STRATEGIE_BASE_ROULANTE_H

#include <cstdint>

#define FREQ_BASE 10000
#define DIAMETRE 60
#define DIV_DRIVER 2
#define DIAMETRE_BASE 93


typedef union {
    struct {
        uint16_t vitesse : 16;
        uint16_t distance : 16;
        uint8_t direction : 8;
    } fields;
    uint64_t unified;
    uint8_t raw_data[8];
} frame_br_t;

typedef union {
    struct {
        uint32_t nb_steps : 32;
        uint8_t div0 : 8;
        uint8_t div1 : 8;
        uint8_t div2 : 8;
        bool dir0 : 1;
        bool dir1 : 1;
        bool dir2 : 1;
    } fields;
    uint64_t unified;
    uint8_t raw_data[8];
} motor_frame_t;


motor_frame_t* convert_frame(frame_br_t* frame_br, motor_frame_t* motor_frame);
float compute_arc(float angle);

#endif //STRATEGIE_BASE_ROULANTE_H
