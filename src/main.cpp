#include <iostream>
#include "Sway.h"

int main() {
    Sway sway_ipc;
    auto sway_outputs = sway_ipc.get_outputs();

    return 0;
}
