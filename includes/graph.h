#ifndef GRAPH_H
#define GRAPH_H

#include <robotech/can.h>
#include <robotech/xbee.h>


class Graph {
private:
    bool *state;
    char **steps;
    char step_count;

    Logger *logs;
    std::mutex mut;
public:
    explicit Graph(Logger *logs, char step_count);

    Graph &set_step(char step);
    Graph &clear_step(char step);
    Graph &set_prev(char step, char* prev);

    bool can_switch(char from, char to);
    void send_state(Can &can, CAN_ADDR addr);
};

#endif
