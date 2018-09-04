//
// Created by jacksoncoder on 9/4/18.
//

#ifndef DUNK_SERVER_H
#define DUNK_SERVER_H



#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string>
#include <fcntl.h>

using namespace std;

class UDPClient {
    sockaddr_in roborio_addr;
    int socket_i;
    bool connected = false;

    public:
        UDPClient(string host, uint16_t port);
        void send(string msg);
};


#endif //DUNK_SERVER_H
