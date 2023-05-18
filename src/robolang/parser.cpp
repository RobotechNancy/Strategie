//
// Created by mrspaar on 5/14/23.
//

#include <fstream>
#include <sstream>
#include <iterator>

#include "robolang/parser.h"


void Parser::parse_config(const char *filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        *logger << "Erreur lors de l'ouverture du fichier " << filename << mendl;
        exit(1);
    }

    std::vector<std::string> lines;
    validate_file(
            std::string((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>()), lines
    );

    for (std::string line: lines) {
        if (line.empty() || line[0] == '\n' || line[0] == '/')
            continue;

        size_t pos = line.find(' ');
        config[line.substr(0, pos)] = line.substr(pos + 1);
    }
}


void Parser::parse_strategy(const char *filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        *logger << "Erreur lors de l'ouverture du fichier " << filename << mendl;
        exit(1);
    }

    std::vector<std::string> lines;
    validate_file(
            std::string((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>()), lines
    );

    for (std::string line: lines) {
        if (line.empty() || line[0] == '\n' || line[0] == '/')
            continue;

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


long Parser::parse_int(const std::string &str) {
    try {
        return std::stoi(str);
    } catch (std::invalid_argument &e) {
        *logger << "Impossible de convert_frame '" << str << "' en entier" << mendl;
        exit(1);
    } catch (std::out_of_range &e) {
        *logger << "La valeur '" << str << "' est trop grande pour être convertie en entier" << mendl;
        exit(1);
    }
}


uint16_t Parser::compute_crc(const std::string &str, uint16_t start, uint16_t end) {
    uint16_t crc = 0xFFFF, count = start;
    const uint16_t POLYNOME = 0xA001;

    do {
        crc ^= str[count];

        for (uint8_t i = 0; i < 8; i++)
            if ((crc % 2) != 0)
                crc = (crc >> 1) ^ POLYNOME;
            else
                crc = (crc >> 1);
    } while (++count < end);

    return crc;
}


void Parser::update_crc(const char *filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();

    size_t start = content.find("START") + 6;
    size_t end = content.find("END") - 2;

    uint16_t crc = Parser::compute_crc(content, start+6, end);
    std::stringstream stream;
    stream << std::hex << crc;
    content.replace(start, 4, stream.str());

    std::ofstream outfile(filename);
    outfile << content;
}


void Parser::validate_file(const std::basic_string<char>& content, std::vector<std::string> &lines) {
    size_t start = content.find("START") + 6;
    size_t end = content.find("END") - 2;

    uint16_t file_crc = std::stoi(content.substr(start, start+4), nullptr, 16);
    uint16_t computed_crc = Parser::compute_crc(content, start+6, end);

    if (file_crc != computed_crc) {
        std::cerr << std::showbase << std::hex << "Le fichier est invalide (" << file_crc << " != " << computed_crc << ")" << std::endl;
        exit(1);
    }

    std::stringstream stream(content.substr(start+6, end-start-6));
    for (std::string line; std::getline(stream, line);)
        lines.push_back(line);
}
