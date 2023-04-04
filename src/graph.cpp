#include <mutex>

#include "graph.h"
using namespace std;


Graph::Graph(Logger *logs, char step_count) {
    this->state = new bool[step_count];

    for (int i = 0; i < step_count; i++)
        this->state[i] = false;

    this->steps = new char*[step_count];
    this->step_count = step_count;
    this->logs = logs;
}


Graph &Graph::set_step(char step) {
    mut.lock();
    state[step-1] = true;
    mut.unlock();

    *logs << "Etape " << int(step) << " activée" << mendl;
    return *this;
}


Graph &Graph::clear_step(char step) {
    mut.lock();
    state[step-1] = false;
    mut.unlock();

    *logs << "Etape " << int(step) << " désactivée" << mendl;
    return *this;
}


Graph &Graph::set_prev(char step, char *prev) {
    mut.lock();
    steps[step-1] = prev;
    mut.unlock();
    return *this;
}


bool Graph::can_switch(char from, char to) {
    mut.lock();

    if (!state[from-1]) {
        *logs << "Demande refusée (pas d'étape n°" << int(from) << ")" << mendl;
        mut.unlock();
        return false;
    }

    if (steps[to-1] == nullptr) {
        *logs << "Demande refusée (pas de prérequis pour l'étape n°" << int(to) << ")" << mendl;
        mut.unlock();
        return false;
    }

    char i=0;

    while (steps[to-1][i] != 0) {
        if (steps[to-1][i] == from) {
            *logs << "Demande acceptée (étape n°" << int(from) << " -> étape n°" << int(to) << ")" << mendl;
            mut.unlock();
            return true;
        }

        i++;
    }

    *logs << "Demande refusée (étape n°" << int(from) << " -> étape n°" << int(to) << ")" << mendl;
    mut.unlock();
    return false;
}


void Graph::send_state(Can &can, CAN_ADDR addr) {
    mut.lock();

    unsigned int len = step_count/8 + (step_count%8==0? 0:1);
    auto *data = new unsigned char[len];

    for (int i=0; i < len*8; i++)
        if (i < step_count && state[i])
            data[i/8] |= 1 << (i % 8);
        else
            data[i/8] &= ~(1 << (i%8));

    can.send(addr, FCT_CHANGEMENT_ETAT, data, len, false, 0, 0);
    mut.unlock();
}
