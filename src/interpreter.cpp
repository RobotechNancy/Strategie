//
// Created by mrspaar on 5/15/23.
//

#include <iostream>
#include "parser.h"
#include "interpreter.h"


void Interpreter::execute(const std::string& name, const std::vector<std::string>& args) {
    if (name == "INIT_COMM") return init_comm();

    if (can == nullptr) {
        *logger << "Erreur : le bus CAN n'a pas été initialisé" << mendl;
        return;
    }

    if (name == "MOVE") return move(args);
    if (name == "OUVRIR_PANIER") return open_basket();
    if (name == "FERMER_PANIER") return close_basket();
    if (name == "ASPIRER") return suck_up();
    if (name == "PLACER_BALLE") return place_ball();
}


void Interpreter::run() {
    for (const auto& instruction : parser.get_instructions()) {
        execute(instruction.first, instruction.second);
    }
}


void Interpreter::init_comm() {
    if (lcd.init() != 0) {
        *logger << "Erreur : impossible d'initialiser l'écran LCD" << mendl;
        exit(1);
    }

    can = std::make_unique<Can>(
            parser.get<uint8_t>("can_address")
    );

    if (can->init() != 0) {
        *logger << "Erreur : impossible d'initialiser le bus CAN" << mendl;
        exit(1);
    }

    xbee = std::make_unique<XBee>(
            parser.get<uint8_t>("xbee_address")
    );

    int status = xbee->openSerialConnection(
            parser.get<std::string>("xbee_port").c_str()
    );

    if (status != 0) {
        *logger << "Erreur : impossible d'initialiser le module XBee" << mendl;
        exit(1);
    }
}


void Interpreter::display(const std::vector<std::string> &args) {
    if (args.size() > 2) {
        *logger << "Erreur : Trop de lignes à afficher (2 max)" << mendl;
        return;
    }

    lcd.clear();

    for (int i = 0; i < args.size(); i++) {
        lcd.set_cursor(i, 0);
        lcd.print(args[i]);
    }
}


void Interpreter::move(const std::vector<std::string> &args) {
    uint16_t speed = parser.parse_int(args[0]);
    uint16_t distance = parser.parse_int(args[1]);
    uint8_t direction = parser.parse_int(args[2]);

    uint8_t data[5] = {
        (uint8_t) (speed >> 8),
        (uint8_t) speed,
        (uint8_t) (distance >> 8),
        (uint8_t) distance,
        direction
    };

    can->send(CAN_ADDR_BASE_ROULANTE, FCT_AVANCE, data, 5, false, 0, 0);
}


void Interpreter::open_basket() {
    can->send(CAN_ADDR_ACTIONNEUR_PETIT, FCT_OUVRIR_PANIER, nullptr, 0, false, 0, 1);
}

void Interpreter::close_basket() {
    can->send(CAN_ADDR_ACTIONNEUR_PETIT, FCT_FERMER_PANIER, nullptr, 0, false, 0, 1);
}

void Interpreter::suck_up() {
    can->send(CAN_ADDR_ACTIONNEUR_PETIT, FCT_ASPIRER_BALLE, nullptr, 0, false, 0, 1);
}

void Interpreter::place_ball() {
    can->send(CAN_ADDR_ACTIONNEUR_PETIT, FCT_PLACER_BALLE, nullptr, 0, false, 0, 1);
}
