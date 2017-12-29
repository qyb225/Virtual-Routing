#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_utils.h"

typedef unsigned char byte;

int init_socket(in_port_t port);
struct sockaddr_in set_dest_addr(const char *dest_addr_str, in_port_t dest_port);

void send_data(int sockfd, const char *dest_addr_str, 
               in_port_t dest_port, void *data_send, int data_len);


#endif