#include "robolang/interpreter.h"

int main() {
    Interpreter interpreter(
            std::make_shared<Logger>("system")
    );

    interpreter.prepare("../data/config.txt", "../data/strategie.txt");
    interpreter.run();

    return 0;
}
