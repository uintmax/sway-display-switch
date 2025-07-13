#ifndef SWAY_H
#define SWAY_H

#include <cstdlib>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SwayOutput {
    std::string name;
    bool active;
};

class Sway {
public:
    Sway();

    std::vector<SwayOutput> get_outputs();

    ~Sway();

private:
    static constexpr const char *SWAYSOCK_VAR = "SWAYSOCK";
    static constexpr std::string PAYLOAD_MAGIC_BYTES = "i3-ipc";
    // Add two 32-bit integers: Payload length and payload type
    static constexpr auto PAYLOAD_HEADER_LEN = PAYLOAD_MAGIC_BYTES.size() + sizeof(uint32_t) * 2;
    static constexpr auto PAYLOAD_LENGTH_POS = 6;
    static constexpr auto PAYLOAD_TYPE_POS = 10;
    static constexpr std::uint32_t MESSAGE_GET_OUTPUTS = 3; // TODO: Create message enum
    static constexpr std::string SWAY_OUTPUT_NAME = "name";
    static constexpr std::string SWAY_OUTPUT_ACTIVE = "active";
    int socket_fd;
};


#endif //SWAY_H
