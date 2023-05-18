//
// Created by mrspaar on 5/15/23.
//

#ifndef GRAPH_INSTRUCTIONS_H
#define GRAPH_INSTRUCTIONS_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <functional>

#include "hardware/lcd.h"
#include "robotech/can.h"
#include "robotech/xbee.h"
#include "robolang/parser.h"
#include "hardware/base_roulante.h"


class Interpreter {
public:
    explicit Interpreter(const std::shared_ptr<Logger>& logger):
        logger(logger), parser(logger), lcd(), br_params() {};

    void init();
    void load_config(const char* configFile) { parser.parse_config(configFile); }
    void load_strategy(const char* stratFile) { parser.parse_strategy(stratFile); }

    void run();
    void execute(const std::string& name, const std::vector<std::string>& args);
private:
    LCD lcd;
    Parser parser;
    br_params_t br_params;
    std::unique_ptr<Can> can;
    std::unique_ptr<XBee> xbee;
    std::shared_ptr<Logger> logger;

    void init_comm();
    void test_comm();
    void display(const std::vector<std::string>& args);
    void move(const std::vector<std::string>& args);
    void open_basket();
    void close_basket();
    void suck_up();
    void place_ball();
    void lower_cog();
    void raise_cog();
    void catch_cake();
    void release_cake();
};


#endif //GRAPH_INSTRUCTIONS_H
