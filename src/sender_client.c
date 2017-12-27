#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "socket_utils.h"
#include "sender_client.h"

void client_send_msg(in_port_t src_port, const char *dest_addr_str, 
                     in_port_t dest_port, const char *msg) {
    byte path_buffer[128];
    byte ip_packet[BUFLEN];
    int path_data_len;
    int sockfd = init_socket(src_port);

    /*ask controller and get path info*/
    ask_controller(sockfd, CTRL_ADDR, CTRL_PORT, HOST_ADDR, src_port, 
                   dest_addr_str, dest_port, path_buffer, &path_data_len);
    
    /*Package*/
    int offset = 0;
    memset(ip_packet, 0, BUFLEN);

    /*src_ip: 32bits, 4B*/
    uint32_t packet_src_addr;
    inet_pton(AF_INET, HOST_ADDR, &packet_src_addr);
    memcpy(ip_packet + offset, &packet_src_addr, 4);
    offset += 4;

    /*src_port: 16bits, 2B*/
    uint16_t packet_src_port = htons(src_port);
    memcpy(ip_packet + offset, &packet_src_port, 2);
    offset += 2;

    uint32_t packet_dest_addr;
    inet_pton(AF_INET, dest_addr_str, &packet_dest_addr);
    memcpy(ip_packet + offset, &packet_dest_addr, 4);
    offset += 4;

    uint16_t packet_dest_port = htons(dest_port);
    memcpy(ip_packet + offset, &packet_dest_port, 2);
    offset += 2;

    /*data_len: 32bits, 4B*/
    uint32_t packet_data_len = strlen(msg) + 1;
    memcpy(ip_packet + offset, &packet_data_len, 4);
    offset += 4;

    /*data: data_len * 8 bits, data_len B*/
    memcpy(ip_packet + offset, msg, packet_data_len);
    offset += packet_data_len;

    /*path: path_data_len * 8 bits, path_data_len B*/
    memcpy(ip_packet + offset, path_buffer, path_data_len);
    offset += path_data_len;

    /*Send to next host*/
    struct in_addr next_in_addr;
    uint16_t next_port;
    offset -= 6;
    memcpy(&(next_in_addr.s_addr), ip_packet + offset, 4);
    memcpy(&next_port, ip_packet + offset + 4, 2);

    printf("next: %s:%d\n", inet_ntoa(next_in_addr), ntohs(next_port));

    send_data(sockfd, inet_ntoa(next_in_addr), ntohs(next_port), ip_packet, offset);
}

void ask_controller(int sockfd, const char *controller_addr_str, in_port_t controller_port,
                    const char *src_addr_str, in_port_t src_port,
                    const char *dest_addr_str, in_port_t dest_port,
                    byte *path_buffer, int *path_data_len) {

    struct search_packet ask_packet;
    
    /*Packet*/
    inet_pton(AF_INET, src_addr_str, &(ask_packet.src_ip_addr));
    ask_packet.src_port = htons(src_port);
    inet_pton(AF_INET, src_addr_str, &(ask_packet.dest_ip_addr));
    ask_packet.dest_port = htons(dest_port);

    /*Ask for path*/
    send_data(sockfd, controller_addr_str, 
              controller_port, &ask_packet, sizeof(ask_packet));
    
    /*recv path_packet...*/
    // memset(buffer, 0, BUFLEN);
    struct sockaddr_in ctrl_addr;
    socklen_t ctrl_addr_len = 0;
    *path_data_len = recvfrom(sockfd, path_buffer, BUFLEN, 0,
                     (struct sockaddr *)&ctrl_addr, &ctrl_addr_len);
}

int main() {
    client_send_msg(8000, "127.0.0.1", 8003, "hello, v-router!");

    return 0;
}