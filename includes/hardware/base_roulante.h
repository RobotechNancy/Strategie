//
// Created by mrspaar on 5/18/23.
//

#ifndef STRATEGIE_BASE_ROULANTE_H
#define STRATEGIE_BASE_ROULANTE_H

#include <cstdint>


typedef struct {
    uint8_t wheel_diamater;
    uint16_t frequency;
    uint8_t divider;
    uint8_t base_diameter;
} br_params_t;

typedef union {
    struct {
        uint16_t speed : 16;
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


motor_frame_t* convert_frame(br_params_t *params, frame_br_t* frame_br, motor_frame_t* motor_frame);
float compute_arc(br_params_t *params, float angle);

#endif //STRATEGIE_BASE_ROULANTE_H
