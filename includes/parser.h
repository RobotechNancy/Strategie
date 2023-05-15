//
// Created by mrspaar on 5/14/23.
//

#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <map>
#include <vector>

#include "instructions.h"


class Parser {
public:
    explicit Parser(const char* filename);

    template<typename T> inline void get(const std::string& key, T* buffer);
    void execute();
private:
    std::vector<Instruction> instructions;
    std::map<std::string, std::string> config;
};


template<typename T>
inline void Parser::get(const std::string &key, T* buffer) {
    if (config.find(key) == config.end()) {
        std::cerr << "La clé " << key << " n'existe pas dans le fichier de configuration" << std::endl;
        exit(1);
    }

    std::stringstream ss(config[key]);
    ss >> *buffer;
}


#endif //GRAPH_PARSER_H
