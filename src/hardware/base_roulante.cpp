/*
 * Convertion depuis les unités du système internationnal vers les unités de la base roulante
 */

#include <cmath>
#include "hardware/base_roulante.h"


motor_frame_t *convert_frame(br_params_t *params, frame_br_t *frame_br, motor_frame_t *motor_frame) {
    const int DIRECTION = frame_br->fields.direction;

    auto dist_per_turn = (float) (params->wheel_diamater * M_PI); // mm
    float step_size = (float) 360.0f / ((float) (params->divider) * 200); // °
    float wished_dist;

    // Si le robot effectue une rotation sur  lui-même
    if ((DIRECTION == 7) || (DIRECTION == 8)) {
        wished_dist = compute_arc(params, frame_br->fields.distance); // mm
    } else {
        wished_dist = frame_br->fields.distance; // mm
    }
    float wished_speed = frame_br->fields.speed; // mm/sec

    // Nb de tours théoriques qu'une roue doit effectuer
    float turn_count = (float) wished_dist / (float) dist_per_turn;

    // Nb de pas théoriques qui seront effectués
    float step_count = turn_count * 360 / step_size;

    bool dir[3];
    float ratio[3];

    // Selon la direction reçue on affecte un sens de rotation et un ratio a chaque roue
    switch (DIRECTION) {
        // Avance angle 0
        case 1:
            dir[0] = false;
            dir[1] = true;
            dir[2] = false;
            ratio[0] = 0;
            ratio[1] = (float) sin(60 * M_PI / 180);
            ratio[2] = (float) sin(2 * 60 * M_PI / 180);
            break;
        // Avance angle 60
        case 2:
            dir[0] = false;
            dir[1] = true;
            dir[2] = false;
            ratio[0] = (float) sin(60 * M_PI / 180);
            ratio[1] = (float) sin(2 * 60 * M_PI / 180);
            ratio[2] = 0;
            break;
        // Avance angle 120
        case 3:
            dir[0] = false;
            dir[1] = false;
            dir[2] = true;
            ratio[0] = (float) sin(2 * 60 * M_PI / 180);
            ratio[1] = 0;
            ratio[2] = (float) sin(60 * M_PI / 180);
            break;
        // Avance angle 180
        case 4:
            dir[0] = false;
            dir[1] = false;
            dir[2] = true;
            ratio[0] = 0;
            ratio[1] = (float) sin(60 * M_PI / 180);
            ratio[2] = (float) sin(2 * 60 * M_PI / 180);
            break;
        // Avance angle 240
        case 5:
            dir[0] = true;
            dir[1] = false;
            dir[2] = false;
            ratio[0] = (float) sin(60 * M_PI / 180);
            ratio[1] = (float) sin(2 * 60 * M_PI / 180);
            ratio[2] = 0;
            break;
        // Avance angle 300
        case 6:
            dir[0] = false;
            dir[1] = true;
            dir[2] = true;
            ratio[0] = (float) sin(2 * 60 * M_PI / 180);
            ratio[1] = 0;
            ratio[2] = (float) sin(60 * M_PI / 180);
            break;
        // Rotation trigo
        case 7:
            dir[0] = true;
            dir[1] = true;
            dir[2] = true;
            ratio[0] = 1;
            ratio[1] = 1;
            ratio[2] = 1;
            break;
        // Rotation anti-trigo
        case 8:
            dir[0] = false;
            dir[1] = false;
            dir[2] = false;
            ratio[0] = 1;
            ratio[1] = 1;
            ratio[2] = 1;
            break;
        default:;
            break;
    }

    // Peut-etre mettre dans un table pour avoir une taille de pas par roue
    float step_frequency = std::round(wished_speed / dist_per_turn * (float) (360) / step_size);

    // Frequence de chaque roue
    float freq[3];

    for (int i = 0; i < 3; i++) {
        if (ratio[i] != 0) {
            freq[i] = step_frequency / ratio[i];
        } else {
            freq[i] = 0;
        }
    }

    float div[3]; //diviseur de chaque roue
    for (int i = 0; i < 3; i++) {
        if (freq[i] != 0) {
            div[i] = (float) params->frequency / freq[i];
        } else {
            div[i] = 0;
        }
    }

    // Nombre de pas que chaque roue doit effectuer
    float wheel_step_count[3];

    for (int i = 0; i < 3; i++)
        if (ratio[i] != 0)
            wheel_step_count[i] = (step_count * div[i] / ratio[i]);
        else
            wheel_step_count[i] = 0;

    // Le nombre de pas total, on le récupère à partir
    if ((DIRECTION == 7) || (DIRECTION == 8)) {
        if (wheel_step_count[0] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std::round(wheel_step_count[0])) + (0.1875 * (std::round(wheel_step_count[0]))));
        } else if (wheel_step_count[1] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std:: round(wheel_step_count[1])) + (0.1875 * (std::round(wheel_step_count[1]))));
        } else if (wheel_step_count[2] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std::round(wheel_step_count[2])) + (0.1875 * (std::round(wheel_step_count[2]))));
        } else {
            motor_frame->fields.nb_steps = 0;
        }
    } else {
        if (wheel_step_count[0] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std::round(wheel_step_count[0])) - (0.2 * (std::round(wheel_step_count[0]))));
        } else if (wheel_step_count[1] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std::round(wheel_step_count[1])) - (0.2 * (std::round(wheel_step_count[1]))));
        } else if (wheel_step_count[2] != 0) {
            motor_frame->fields.nb_steps = (uint32_t) ((std::round(wheel_step_count[2])) - (0.2 * (std::round(wheel_step_count[2]))));
        } else {
            motor_frame->fields.nb_steps = 0;
        }
    }

    motor_frame->fields.div0 = (uint32_t) (std::round(div[0]));
    motor_frame->fields.div1 = (uint32_t) (std::round(div[1]));
    motor_frame->fields.div2 = (uint32_t) (std::round(div[2]));
    motor_frame->fields.dir0 = (uint32_t) (std::round(dir[0]));
    motor_frame->fields.dir1 = (uint32_t) (std::round(dir[1]));
    motor_frame->fields.dir2 = (uint32_t) (std::round(dir[2]));

    return motor_frame;
}


float compute_arc(br_params_t *params, float angle) {
    float arc = (float) (2 * M_PI * (params->base_diameter) * angle) / 360;
    return arc;
}