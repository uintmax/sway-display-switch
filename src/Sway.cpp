#include "Sway.h"

Sway::Sway() {
    const char *socket_path = std::getenv(swaysock_var);
    if (!socket_path)
        throw std::runtime_error("Sway socket path environment variable is empty");

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1)
        throw std::runtime_error("Could not create socket");

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    size_t max_buffer_len = sizeof(addr.sun_path) - 1;
    std::strncpy(addr.sun_path, socket_path, max_buffer_len);
    int conn = connect(socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(sockaddr_un));
    if (conn == -1)
        throw std::runtime_error("Could not connect to Sway socket");
}

std::vector<SwayOutput> Sway::get_outputs() {
    send_packet({SwayMsg::get_outputs});
    SwayPacket response_outputs = receive_packet();

    if (response_outputs.sway_msg != SwayMsg::get_outputs)
        throw std::runtime_error("Message reply type did not match get_outputs type");

    json json_outputs = json::parse(response_outputs.payload);

    std::vector<SwayOutput> outputs;
    for (const auto &json_output: json_outputs) {
        auto output_name = json_output.at(sway_output_name).get<std::string>();
        auto output_active = json_output.at(sway_output_active).get<bool>();
        outputs.emplace_back(output_name, output_active);
    }
    return outputs;
}

bool Sway::set_output(const std::string &name, bool state) {
    std::string command;
    if (state)
        command = std::format(sway_output_enable_fmt, name);
    else
        command = std::format(sway_output_disable_fmt, name);

    send_packet({SwayMsg::run_command, command});
    SwayPacket response_set_output = receive_packet();
    if (response_set_output.sway_msg != SwayMsg::run_command)
        throw std::runtime_error("Message reply type did not match run_command type");

    json json_response = json::parse(response_set_output.payload);
    auto success = json_response.begin().value().at("success").get<bool>();
    return success;
}

void Sway::send_packet(const SwayPacket &sway_packet) {
    SwayMsg sway_msg = sway_packet.sway_msg;
    std::string payload = sway_packet.payload;
    std::vector<uint8_t> packet(payload_header_len + payload.length());
    payload_magic_bytes.copy(reinterpret_cast<char *>(packet.data()), payload_magic_bytes.size());
    // TODO: Better serialization method? Maybe extra struct
    *reinterpret_cast<uint32_t *>(&packet[payload_length_pos]) = payload.length();
    *reinterpret_cast<uint32_t *>(&packet[payload_type_pos]) = std::to_underlying(sway_msg);
    if (!payload.empty())
        payload.copy(reinterpret_cast<char *>(&packet[payload_header_len]), payload.length());

    auto bytes_written = write(socket_fd, packet.data(), packet.size());
    if (bytes_written == -1)
        throw std::runtime_error("Error writing to Sway socket");
    if (bytes_written != payload_header_len + sway_packet.payload.length())
        throw std::runtime_error(
            "Only wrote " + std::to_string(bytes_written) + " out of " + std::to_string(payload_header_len) +
            " bytes to Sway socket");
}

Sway::SwayPacket Sway::receive_packet() {
    std::uint8_t reply_header_buffer[payload_header_len] = {};
    auto header_bytes_read = read(socket_fd, reply_header_buffer, payload_header_len);
    if (header_bytes_read == -1)
        throw std::runtime_error("Error reading from Sway socket");
    if (header_bytes_read != payload_header_len)
        throw std::runtime_error(
            "Only read " + std::to_string(header_bytes_read) + " out of " + std::to_string(payload_header_len) +
            " bytes from Sway socket");

    if (std::memcmp(reply_header_buffer, payload_magic_bytes.data(), payload_magic_bytes.size()) != 0)
        throw std::runtime_error("Reply does not start with magic bytes");

    uint32_t reply_len = *reinterpret_cast<uint32_t *>(&reply_header_buffer[payload_length_pos]);
    uint32_t reply_type = *reinterpret_cast<uint32_t *>(&reply_header_buffer[payload_type_pos]);

    std::vector<uint8_t> reply_payload_buffer(reply_len);
    auto payload_bytes_read = read(socket_fd, reply_payload_buffer.data(), reply_len);
    if (payload_bytes_read == -1)
        throw std::runtime_error("Error reading from Sway socket");
    if (payload_bytes_read != reply_len)
        throw std::runtime_error(
            "Only read " + std::to_string(payload_bytes_read) + " out of " + std::to_string(reply_len) +
            " bytes from Sway socket");

    return SwayPacket(static_cast<SwayMsg>(reply_type),
                      std::string(reply_payload_buffer.begin(), reply_payload_buffer.end()));
}

Sway::~Sway() {
    close(socket_fd);
}
