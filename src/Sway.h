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
#include <format>
#include <utility>

using json = nlohmann::json;

struct SwayOutput {
    std::string name;
    bool active;
};


/**
 * Abstraction for Sway IPC protcol
 */
class Sway {
public:
    Sway();

    std::vector<SwayOutput> get_outputs();

    bool set_output(const std::string &name, bool state);

    ~Sway();

private:
    enum class SwayMsg {
        run_command = 0,
        get_outputs = 3,
    };

    struct SwayPacket {
        SwayMsg sway_msg;
        std::string payload = "";
    };

    void send_packet(const SwayPacket &sway_packet);

    SwayPacket receive_packet();


    static constexpr const char *swaysock_var = "SWAYSOCK";
    static constexpr std::string_view payload_magic_bytes = "i3-ipc";
    // Add two 32-bit integers: Payload length and payload type
    static constexpr auto payload_header_len = payload_magic_bytes.size() + sizeof(uint32_t) * 2;
    static constexpr auto payload_length_pos = 6;
    static constexpr auto payload_type_pos = 10;
    static constexpr std::string_view sway_output_name = "name";
    static constexpr std::string_view sway_output_active = "active";
    static constexpr std::string_view sway_output_enable_fmt = "output {} enable";
    static constexpr std::string_view sway_output_disable_fmt = "output {} disable";
    int socket_fd;
};


#endif //SWAY_H
