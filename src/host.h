#ifndef HOST_H
#define HOST_H

#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define HOST_ADDR "127.0.0.1"
#define BUFLEN 1024

void receive_forward(in_port_t port);

#endif