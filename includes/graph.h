#ifndef GRAPH_H
#define GRAPH_H

#include "robotech/logs.h"


class Graph {
private:
    bool *state;
    char **steps;
    char step_count;

    Logger logs;
    std::mutex mut;
public:
    explicit Graph(char step_count);

    Graph &set_step(char step);
    Graph &clear_step(char step);
    Graph &set_prev(char step, char* prev);
    bool can_switch(char from, char to);
};

#endif
