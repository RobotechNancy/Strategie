//
// Created by mrspaar on 5/15/23.
//

#include <iostream>
#include "instructions.h"


Instruction::Instruction(const std::string& name, std::vector<std::string> args) {
    this->args = std::move(args);

    if (name == "MOVE") callback = Instructions::move;
    else {
        std::cerr << "L'instruction " << name << " n'existe pas" << std::endl;
        exit(1);
    }
}


void Instructions::move(const std::vector<std::string> &args) {
    std::cout << "MOVE" << std::endl;
}
