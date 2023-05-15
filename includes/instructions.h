//
// Created by mrspaar on 5/15/23.
//

#ifndef GRAPH_INSTRUCTIONS_H
#define GRAPH_INSTRUCTIONS_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <functional>


// ===================================== INSTRUCTIONS =====================================

namespace Instructions {
    void move(const std::vector<std::string>& args);
}

// ====================================== LIBRARY =========================================

typedef std::function<void(std::vector<std::string>)> callback_t;

struct Instruction {
    callback_t callback;
    std::vector<std::string> args;

    Instruction(const std::string& name, std::vector<std::string> args);
};

// ========================================================================================

#endif //GRAPH_INSTRUCTIONS_H
