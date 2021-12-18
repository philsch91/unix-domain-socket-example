//
//  server.c
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
    printf("Server\n");
    struct sockaddr_un addr;
    struct sockaddr_un from;
    socklen_t fromlen = sizeof(from);
    char buffer[8192];
    int len, ret, fd;
    
    if ((fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        return 1;
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_SOCKET_FILE);
    unlink(SERVER_SOCKET_FILE);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(fd);
        perror("Bind()");
        return 1;
    }

    while ((len = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &fromlen)) > 0) {
        printf("received %d bytes: %s\n", len, buffer);
        strcpy(buffer, "transmission good");
        ret = sendto(fd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&from, fromlen);
        if (ret < 0) {
            perror("sendto()");
            break;
        }
    }

    close(fd);

    return 0;
}
