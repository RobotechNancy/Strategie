//
// Created by mrspaar on 5/14/23.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "parser.h"


Parser::Parser(const std::shared_ptr<Logger>& logger, const char *filename): logger(logger) {
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

    std::cout << "Fichier " << filename << " chargé avec succès" << std::endl;
}


long Parser::parse_int(const std::string &str) {
    try {
        return std::stoi(str);
    } catch (std::invalid_argument &e) {
        *logger << "Impossible de convertir '" << str << "' en entier" << mendl;
        exit(1);
    } catch (std::out_of_range &e) {
        *logger << "La valeur '" << str << "' est trop grande pour être convertie en entier" << mendl;
        exit(1);
    }
}
