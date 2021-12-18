//
//  client.c
//  unix-domain-socket-example
//
//  Created by philipp on 18.12.21.
//  Copyright © 2021 philipp. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ipc.h"

int main(int argc, const char * argv[]) {
    printf("Client\n");

    struct sockaddr_un addr;
    struct sockaddr_un from;
    char buffer[8192];
    int ret, len, fd;

    if (argc != 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }

    if ((fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, CLIENT_SOCKET_FILE);
    unlink(CLIENT_SOCKET_FILE);
    
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind()");
        close(fd);
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_SOCKET_FILE);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect()");
        close(fd);
        return 1;
    }

    strcpy(buffer, argv[1]);
    if (send(fd, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("send()");
        close(fd);
        return 1;
    }

    printf("sent: %s\n", argv[1]);

    if ((len = recv(fd, buffer, sizeof(buffer), 0)) < 0) {
        perror("recv()");
    } else {
        printf("received %d bytes: %s\n", len, buffer);
    }

    close(fd);
    unlink(CLIENT_SOCKET_FILE);

    return 0;
}
