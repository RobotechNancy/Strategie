#include "interpreter.h"

int main() {
    std::shared_ptr<Logger> logger = std::make_shared<Logger>("system");
    Interpreter interpreter(logger, "instructions.txt");

    return 0;
}
