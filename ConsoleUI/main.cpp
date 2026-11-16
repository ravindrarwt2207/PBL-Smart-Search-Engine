
#include <exception>
#include <iostream>

#include "ConsoleUI.h"

int main() {
    try {
        ConsoleUI ui;
        ui.run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Fatal error: unknown problem.\n";
        return 1;
    }
    return 0;
}
