//
// Created by mrspaar on 5/14/23.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "parser.h"


Parser::Parser(const char *filename) {
    std::ifstream file(filename);
    bool inData = true;

    if (!file.is_open()) {
        std::cout << "Erreur lors de l'ouverture du fichier " << filename << std::endl;
        return;
    }

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.empty() || line[0] == '\n' || line[0] == '/' || line == "#DATA")
            continue;

        if (line == "#CODE") {
            inData = false;
            continue;
        }

        if (inData) {
            size_t pos = line.find('=');
            config[line.substr(0, pos - 1)] = line.substr(pos + 2);
            continue;
        }

        size_t pos = line.find(' ');
        std::istringstream buffer(line.substr(pos + 1));

        std::string name = line.substr(0, pos);
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);

        instructions.emplace_back(
                name,
                std::vector<std::string>(
                        std::istream_iterator<std::string>(buffer),
                        std::istream_iterator<std::string>()
                )
        );
    }
}


void Parser::execute() {
    for (auto &instruction: instructions) {
        instruction.callback(instruction.args);
    }
}
