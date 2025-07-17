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
    static constexpr const char *swaysock_var = "SWAYSOCK";
    static constexpr std::string payload_magic_bytes = "i3-ipc";
    // Add two 32-bit integers: Payload length and payload type
    static constexpr auto payload_header_len = payload_magic_bytes.size() + sizeof(uint32_t) * 2;
    static constexpr auto payload_length_pos = 6;
    static constexpr auto payload_type_pos = 10;
    static constexpr std::uint32_t message_get_outputs = 3; // TODO: Create message enum
    static constexpr std::string sway_output_name = "name";
    static constexpr std::string sway_output_active = "active";
    int socket_fd;
};


#endif //SWAY_H
