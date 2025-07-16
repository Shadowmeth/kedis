#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

static void do_something(int conn_fd) {
    char rbuf[64] = {};
    ssize_t n = read(conn_fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        std::cerr << "read() error\n";
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(conn_fd, wbuf, strlen(wbuf));
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    const int time_wait = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &time_wait, sizeof(time_wait));

    // set up socket on 0.0.0.0:1234
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        std::cerr << "bind() failed!\n";
        exit(EXIT_FAILURE);
    }


    rv = listen(fd, SOMAXCONN);
    if (rv) {
        std::cerr << "listen() failed!\n";
        exit(EXIT_FAILURE);
    }

    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t client_addr_len = sizeof(client_addr);
        int conn_fd = accept(fd, (sockaddr *)&client_addr, &client_addr_len);
        if (conn_fd < 0) {
            std::cerr << "accept() failed for some reason! retrying!\n";
            continue; // error
        }

        do_something(conn_fd);
        close(conn_fd);
    }
    
    return 0;
}
