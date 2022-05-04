// #include "Broker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include<thread>

#include<iostream>

void client_handler (int sock) {
    ssize_t r;
    while (true) {
        r = send(sock, "Hi\n", 3, 0);
        if (r < 0) break;
        sleep(1);
    }
    close(sock);
}

int make_accept_sock (const char *servspec) {
    const int one = 1;
    struct addrinfo hints = {};
    struct addrinfo *res = 0, *ai = 0, *ai4 = 0;
    char *node = strdup(servspec);
    char *service = strrchr(node, ':');
    int sock;

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    *service++ = '\0';
    getaddrinfo(*node ? node : "0::0", service, &hints, &res);
    free(node);

    for (ai = res; ai; ai = ai->ai_next) {
        if (ai->ai_family == PF_INET6) break;
        else if (ai->ai_family == PF_INET) ai4 = ai;
    }
    ai = ai ? ai : ai4;

    sock = socket(ai->ai_family, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    bind(sock, ai->ai_addr, ai->ai_addrlen);
    listen(sock, 256);
    freeaddrinfo(res);

    return sock;
}

void accept_loop (const char *servspec) {
    int sock = make_accept_sock(servspec);

    for (;;) {
        std::cout << "Waiting for a client to connect..." << std::endl;
        int new_sock = accept(sock, 0, 0);
        std::thread t(client_handler, new_sock);
        t.detach();
    }
}

int main(int argc, char **argv ){
    const char *server = ":11111";

    signal(SIGPIPE, SIG_IGN);

    if (argc > 1) server = argv[1];

    accept_loop(server);

    return 0;
}