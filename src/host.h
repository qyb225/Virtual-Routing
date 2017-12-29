#ifndef HOST_H
#define HOST_H

#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define HOST_ADDR "127.0.0.1"
#define BUFLEN 1024

/*API*/
void receive_forward(in_port_t port);

/*Utils*/
int confirm_dest(byte *packet, uint32_t host_ip_addr, uint16_t host_port_ns);

void get_cut_next_path(byte *packet, int *data_len,
                       uint32_t *next_ip_addr, uint16_t *next_port);

void get_packet_data(byte *packet, uint32_t *src_ip_addr_p, uint16_t *src_port_ns_p,
                     uint32_t *dest_ip_addr_p, uint16_t *dest_port_ns_p, 
                     int *msg_len_p, char **msg_p);
                
void display_data(uint32_t src_ip_addr, uint16_t src_port_ns,
                  uint32_t dest_ip_addr, uint16_t dest_port_ns,
                  int msg_len, const char *msg);

#endif