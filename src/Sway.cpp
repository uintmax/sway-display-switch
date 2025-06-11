#include "Sway.h"

Sway::Sway() {
    const char *socket_path = std::getenv(SWAYSOCK_VAR);
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

Sway::~Sway() {
    close(socket_fd);
}
