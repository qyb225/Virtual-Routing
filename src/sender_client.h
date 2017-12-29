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
/*Ask controller for path packet*/
void ask_controller(int sockfd, const char *controller_addr_str, in_port_t controller_port,
                    const char *src_addr_str, in_port_t src_port,
                    const char *dest_addr_str, in_port_t dest_port, 
                    byte *path_buffer, int *path_seg_len);

/*Pack*/
void set_ask_packet(const char *src_addr_str, in_port_t src_port,
                    const char *dest_addr_str, in_port_t dest_port,
                    byte *ask_packet);

/*set src addr/port, dest addr/port 12B*/
void set_addr_header(const char *src_addr_str, in_port_t src_port,
                     const char *dest_addr_str, in_port_t dest_port,
                     byte *addr_header_seg);

/*Data*/
void set_data(const char *msg, byte *data_seg, int *data_seg_len);

/*Path*/
void set_path(const byte *path_data, byte *path_seg, int path_seg_len);

/*Cut addr seg and send*/
void cut_send(int sockfd, byte *packet, int packet_len);

#endif