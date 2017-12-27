#ifndef SENDER_CLI
#define SENDER_CLI

#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define BUFLEN 1024
#define HOST_ADDR "127.0.0.1"
#define CTRL_ADDR "127.0.0.1"
#define CTRL_PORT 8020

/*API*/
void client_send_msg(in_port_t src_port, const char *dest_addr_str, 
                     in_port_t dest_port, const char *msg);

/*Utils*/
void ask_controller(int sockfd, const char *controller_addr_str, in_port_t controller_port,
                    const char *src_addr_str, in_port_t src_port,
                    const char *dest_addr_str, in_port_t dest_port, 
                    byte *path_buffer, int *path_data_len);

#endif