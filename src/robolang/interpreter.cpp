//
// Created by mrspaar on 5/15/23.
//

#include <iostream>
#include "robolang/interpreter.h"


void Interpreter::execute(const std::string& name, const std::vector<std::string>& args) {
    if (name == "AFFICHER") return display(args);
    if (name == "INIT_COMM") return init_comm();

    if (can == nullptr) {
        *logger << "Erreur : le bus CAN n'a pas été initialisé" << mendl;
        exit(1);
    }

    if (name == "TEST_COMM") return test_comm();
    if (name == "MOVE") return move(args);
    if (name == "OUVRIR_PANIER") return open_basket();
    if (name == "FERMER_PANIER") return close_basket();
    if (name == "ASPIRER") return suck_up();
    if (name == "PLACER_BALLE") return place_ball();
    if (name == "BAISSER_CREMAILLERE") return lower_cog(args);
    if (name == "MONTER_CREMAILLERE") return raise_cog(args);
    if (name == "ATTRAPER_GATEAU") return catch_cake(args);
    if (name == "LACHER_GATEAU") return release_cake(args);

    *logger << "Erreur : instruction inconnue : " << name << mendl;
    exit(1);
}


void Interpreter::init() {
    if (lcd.init() != 0) {
        *logger << "Erreur : impossible d'initialiser l'écran LCD" << mendl;
        exit(1);
    }

    can = std::make_unique<Can>();

    xbee = std::make_unique<XBee>(
            parser.get<uint8_t>("xbee_address")
    );

    br_params = (br_params_t) {
            parser.get<uint8_t>("wheel_diameter"),
            parser.get<uint16_t>("br_frequency"),
            parser.get<uint8_t>("br_divider"),
            parser.get<uint8_t>("br_diameter")
    };
}


void Interpreter::run() {
    for (const auto& instruction : parser.get_instructions()) {
        execute(instruction.first, instruction.second);
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


void Interpreter::init_comm() {
    if (can->init() != 0) {
        *logger << "Erreur : impossible d'initialiser le bus CAN" << mendl;
        exit(1);
    }

    int status = xbee->openSerialConnection(
            parser.get<std::string>("xbee_port").c_str()
    );

    if (status != 0) {
        *logger << "Erreur : impossible d'initialiser le module XBee" << mendl;
        exit(1);
    }
}


void Interpreter::test_comm() {
    uint8_t can_data[1] = {0x02};
    can_mess_t can_resp;

    CAN_ADDR can_addrs[4] = {
            CAN_ADDR_BASE_ROULANTE,
            CAN_ADDR_ODOMETRIE,
            CAN_ADDR_ODOMETRIE_TOF,
            CAN_ADDR_ACTIONNEUR
    };

    for (auto addr: can_addrs) {
        can->send(addr, FCT_ACCUSER_RECPETION, can_data, 1, false, 1, 0);
        can_resp = can->wait_for_response(FCT_ACCUSER_RECPETION, 1000);

        if (can_resp.data[0] != 0x02) {
            *logger << "Erreur : le module " << std::showbase << std::hex << addr << " n'a pas répondu correctement" << mendl;
            exit(1);
        }
    }

    std::vector<uint8_t> xbee_data = {0x02};
    xbee_frame_t xbee_resp;

    uint8_t xbee_addrs[2] = {
            XB_ADR_CAMERA_01,
            XB_ADR_CAMERA_02
    };

    for (auto addr: xbee_addrs) {
        xbee->sendFrame(addr, XB_FCT_TEST_ALIVE, xbee_data, 1);
        xbee_resp = xbee->wait_for_response(XB_FCT_TEST_ALIVE, 1000);

        if (xbee_resp.data[0] != 0x02) {
            *logger << "Erreur : le module XBee " << std::showbase << std::hex << addr << " n'a pas répondu correctement" << mendl;
            exit(1);
        }
    }
}


void Interpreter::move(const std::vector<std::string> &args) {
    frame_br_t frame_br;
    frame_br.fields.speed = parser.parse_int(args[0]);
    frame_br.fields.distance = parser.parse_int(args[1]);
    frame_br.fields.direction = parser.parse_int(args[2]);

    motor_frame_t motor_frame;
    convert_frame(&br_params, &frame_br, &motor_frame);

    can->send(CAN_ADDR_BASE_ROULANTE, FCT_AVANCE, motor_frame.raw_data, 8, false, 1, 0);
}


void Interpreter::open_basket() {
    can->send(CAN_ADDR_ACTIONNEUR, FCT_OUVRIR_PANIER, nullptr, 0, false, 0, 1);
}

void Interpreter::close_basket() {
    can->send(CAN_ADDR_ACTIONNEUR, FCT_FERMER_PANIER, nullptr, 0, false, 0, 1);
}

void Interpreter::suck_up() {
    can->send(CAN_ADDR_ACTIONNEUR, FCT_ASPIRER_BALLE, nullptr, 0, false, 0, 1);
}

void Interpreter::place_ball() {
    can->send(CAN_ADDR_ACTIONNEUR, FCT_PLACER_BALLE, nullptr, 0, false, 0, 1);
}

void Interpreter::lower_cog(const std::vector<std::string>& args) {
    uint8_t data[1] = { (uint8_t) parser.parse_int(args[0]) };
    can->send(CAN_ADDR_ACTIONNEUR, FCT_BAISSER_CREMAILLERE, data, 1, false, 0, 1);
}

void Interpreter::raise_cog(const std::vector<std::string>& args) {
    uint8_t data[1] = { (uint8_t) parser.parse_int(args[0]) };
    can->send(CAN_ADDR_ACTIONNEUR, FCT_MONTER_CREMAILLERE, data, 1, false, 0, 1);
}

void Interpreter::catch_cake(const std::vector<std::string>& args) {
    uint8_t data[1] = { (uint8_t) parser.parse_int(args[0]) };

    can->send(CAN_ADDR_ACTIONNEUR, FCT_ATTRAPER_GATEAU, data, 1, false, 0, 1);
}

void Interpreter::release_cake(const std::vector<std::string>& args) {
    uint8_t data[1] = { (uint8_t) parser.parse_int(args[0]) };
    can->send(CAN_ADDR_ACTIONNEUR, FCT_LACHER_GATEAU, data, 1, false, 0, 1);
}
