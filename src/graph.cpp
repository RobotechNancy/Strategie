#include <mutex>
#include "graph.h"


Graph::Graph(char count): logs("system") {
    state = new bool[step_count];

    for (int i = 0; i < step_count; i++)
        state[i] = false;

    steps = new char*[step_count];
    step_count = count;
}


Graph &Graph::set_step(char step) {
    mut.lock();
    state[step-1] = true;
    mut.unlock();

    logs << "Etape " << int(step) << " activée" << mendl;
    return *this;
}


Graph &Graph::clear_step(char step) {
    mut.lock();
    state[step-1] = false;
    mut.unlock();

    logs << "Etape " << int(step) << " désactivée" << mendl;
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
        logs << "Demande refusée (pas d'étape n°" << int(from) << ")" << mendl;
        mut.unlock();
        return false;
    }

    if (steps[to-1] == nullptr) {
        logs << "Demande refusée (pas de prérequis pour l'étape n°" << int(to) << ")" << mendl;
        mut.unlock();
        return false;
    }

    char i=0;

    while (steps[to-1][i] != 0) {
        if (steps[to-1][i] == from) {
            logs << "Demande acceptée (étape n°" << int(from) << " -> étape n°" << int(to) << ")" << mendl;
            mut.unlock();
            return true;
        }

        i++;
    }

    logs << "Demande refusée (étape n°" << int(from) << " -> étape n°" << int(to) << ")" << mendl;
    mut.unlock();
    return false;
}
