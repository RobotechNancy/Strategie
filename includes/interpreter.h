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

#include "lcd.h"
#include "parser.h"
#include "robotech/can.h"
#include "robotech/xbee.h"


class Interpreter {
public:
    explicit Interpreter(const std::shared_ptr<Logger>& logger, const char* filename):
        logger(logger), parser(logger, filename), lcd() {};

    void run();
    void execute(const std::string& name, const std::vector<std::string>& args);
private:
    LCD lcd;
    Parser parser;
    std::unique_ptr<Can> can;
    std::unique_ptr<XBee> xbee;
    std::shared_ptr<Logger> logger;

    void init_comm();
    void display(const std::vector<std::string>& args);
    void move(const std::vector<std::string>& args);
    void open_basket();
    void close_basket();
    void suck_up();
    void place_ball();
};


#endif //GRAPH_INSTRUCTIONS_H
