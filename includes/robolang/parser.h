//
// Created by mrspaar on 5/14/23.
//

#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <map>
#include <vector>
#include "robotech/can.h"


class Parser {
public:
    explicit Parser(const std::shared_ptr<Logger>& logger): logger(logger) {};

    long parse_int(const std::string& str);
    void parse_config(const char *filename);
    void parse_strategy(const char *filename);

    template<typename T> inline T get(const std::string& key) {
        if (config.find(key) == config.end()) {
            std::cerr << "La clé " << key << " n'existe pas dans le fichier de configuration" << std::endl;
            exit(1);
        }

        T value;
        std::stringstream ss(config[key]);
        ss >> value;

        return value;
    };

    [[nodiscard]] std::vector<std::pair<std::string, std::vector<std::string>>> get_instructions() const {
        return instructions;
    };

    static uint16_t compute_crc(const std::string &str, uint16_t start, uint16_t end);
    static void update_crc(const char* filename);
    static void validate_file(const std::basic_string<char>& str, std::vector<std::string> &lines);
private:
    std::shared_ptr<Logger> logger;
    std::map<std::string, std::string> config;
    std::vector<std::pair<std::string, std::vector<std::string>>> instructions;
};


#endif //GRAPH_PARSER_H
