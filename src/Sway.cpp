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
    std::uint8_t payload[payload_header_len] = {};
    payload_magic_bytes.copy(reinterpret_cast<char *>(payload), payload_magic_bytes.size());
    *reinterpret_cast<uint32_t *>(&payload[payload_length_pos]) = 0;
    *reinterpret_cast<uint32_t *>(&payload[payload_type_pos]) = message_get_outputs;

    auto bytes_written = write(socket_fd, payload, payload_header_len);
    if (bytes_written == -1)
        throw std::runtime_error("Error writing to Sway socket");
    if (bytes_written != payload_header_len)
        throw std::runtime_error(
            "Only wrote " + std::to_string(bytes_written) + " out of " + std::to_string(payload_header_len) +
            " bytes to Sway socket");

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

    if (reply_type != message_get_outputs)
        throw std::runtime_error("Message reply type did not match get_outputs type");

    std::vector<uint8_t> reply_payload_buffer(reply_len);
    auto payload_bytes_read = read(socket_fd, reply_payload_buffer.data(), reply_len);
    if (payload_bytes_read == -1)
        throw std::runtime_error("Error reading from Sway socket");
    if (payload_bytes_read != reply_len)
        throw std::runtime_error(
            "Only read " + std::to_string(payload_bytes_read) + " out of " + std::to_string(reply_len) +
            " bytes from Sway socket");


    char *output_str = reinterpret_cast<char *>(reply_payload_buffer.data());
    std::string test(output_str);
    std::cout << test << std::endl;
    json json_outputs = json::parse(test);

    std::vector<SwayOutput> outputs;
    for (const auto &json_output: json_outputs) {
        auto output_name = json_output.at(sway_output_name).get<std::string>();
        auto output_active = json_output.at(sway_output_active).get<bool>();
        SwayOutput output{output_name, output_active};
        outputs.push_back(output);
    }
    return outputs;
}

bool Sway::set_output(const std::string &name, bool state) {
    std::string command;
    if (state)
        command = std::format(sway_output_enable_fmt, name);
    else
        command = std::format(sway_output_disable_fmt, name);

    // TODO: Extra payload builder class/function
    std::vector<uint8_t> payload(payload_header_len + command.size());
    payload_magic_bytes.copy(reinterpret_cast<char *>(payload.data()), payload_magic_bytes.size());
    *reinterpret_cast<uint32_t *>(&payload[payload_length_pos]) = command.size();
    *reinterpret_cast<uint32_t *>(&payload[payload_type_pos]) = message_run_command;

    command.copy(reinterpret_cast<char *>(&payload[payload_header_len]), command.size());

    auto bytes_written = write(socket_fd, payload.data(), payload.size());
    if (bytes_written == -1)
        throw std::runtime_error("Error writing to Sway socket");
    if (bytes_written != payload.size())
        throw std::runtime_error(
            "Only wrote " + std::to_string(bytes_written) + " out of " + std::to_string(payload_header_len) +
            " bytes to Sway socket");

    // TODO: Read response
    return true;
}

Sway::~Sway() {
    close(socket_fd);
}
