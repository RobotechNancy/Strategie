#include "nodes.h"

using namespace std;


int main() {
    Can can;
    int err = can.init(CAN_ADDR_RASPBERRY_E);

    if (err < 0) {
        can.logger << "Erreur lors de l'initialisation du bus CAN (n°" + to_string(err) + ")";
        return err;
    }

    XBee xbee;
    err = xbee.openSerialConnection(XB_ADR_ROBOT_01);

    if (err != XB_SER_E_SUCCESS) {
        xbee.logger << "Erreur lors de l'ouverture de la connexion série (n°" + to_string(err) + ")";
        return err;
    }

    can.start_listen();
    Graph graph(&can.logger, 10);

    graph.set_prev(1, new char[1]{0});
    graph.set_prev(2, new char[1]{1});
    graph.set_prev(3, new char[2]{2, 7});
    graph.set_prev(4, new char[3]{3, 7, 8});
    graph.set_prev(5, new char[1]{4});
    graph.set_prev(6, new char[1]{1});
    graph.set_prev(7, new char[7]{1, 2, 3, 4, 5, 6, 8});
    graph.set_prev(8, new char[2]{4, 7});
    graph.set_prev(9, new char[1]{5});

    if (graph.can_switch(1, 2)) {
        graph.clear_step(1);
        graph.set_step(2);

        graph.send_state(can, CAN_ADDR_BROADCAST);
    }

    this_thread::sleep_for(chrono::seconds(60));
    return 0;
}
