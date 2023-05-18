#include "robolang/interpreter.h"

int main() {
    Parser::update_crc("../data/strategie.txt");
    Parser::update_crc("../data/config.txt");

    /*Interpreter interpreter(
            std::make_shared<Logger>("system")
    );

    interpreter.load_config("../data/config.txt");
    interpreter.init();
    interpreter.load_strategy("../data/strategie.txt");
    interpreter.run();*/

    return 0;
}
