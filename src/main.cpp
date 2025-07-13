#include <iostream>
#include "Sway.h"

int main() {
    Sway sway_ipc;
    auto sway_outputs = sway_ipc.get_outputs();
    for (const auto &output: sway_outputs) {
        std::cout << "output: " << output.name << ": " << std::boolalpha << output.active << std::endl;
    }

    return 0;
}
