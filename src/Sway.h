#ifndef SWAY_H
#define SWAY_H

#include <cstdlib>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <unistd.h>

class Sway {
public:
    Sway();

    ~Sway();

private:
    static constexpr const char *SWAYSOCK_VAR = "SWAYSOCK";
    int socket_fd;
};


#endif //SWAY_H
