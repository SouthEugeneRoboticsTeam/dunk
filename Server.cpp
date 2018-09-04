//
// Created by jacksoncoder on 9/4/18.
//

#include <cstring>
#include "Server.h"

Server::Server(string host, uint16_t port) {
    socket_i = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_i < 0) {
        perror("Could not create a UDP socket");
        exit(1); // Maybe move exit somewhere else
    }

    bzero(&roborio_addr, sizeof(roborio_addr));
    roborio_addr.sin_family = AF_INET;
    roborio_addr.sin_addr.s_addr = inet_addr(host.c_str());
    roborio_addr.sin_port = htons(port);
    connected = true;
}

void Server::send(string msg) {
    if (!connected) return;
    auto msg_str = msg.c_str();
    if (sendto(socket_i, msg_str, strlen(msg_str)+1, 0, // +1 to include terminator
               (sockaddr*)&roborio_addr, sizeof(roborio_addr)) < 0){
        perror("Could not send UDP message");
        close(socket_i);
        connected = false;
    }
}