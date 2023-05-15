#include "graph.h"
#include "parser.h"


int main() {
    /*Parser parser("../strategie.txt");
    parser.execute();

    int xbee_addr;
    parser.get<int>("xbee_address", &xbee_addr);*/

    /*Can can;
    int err = can.init(CAN_ADDR_RASPBERRY_E);

    if (err < 0) {
        can.logger << "Erreur lors de l'initialisation du bus CAN (n°" + std::to_string(err) + ")";
        return err;
    }

    XBee xbee;
    err = xbee.openSerialConnection("/dev/ttyS0", XB_ADR_ROBOT_01);

    if (err != XB_SER_E_SUCCESS) {
        std::cout << "Erreur lors de l'ouverture de la connexion série (n°" + std::to_string(err) + ")";
        return err;
    }

    can.start_listen();
    xbee.start_listen();

    while (true);
    return 0;*/
}
