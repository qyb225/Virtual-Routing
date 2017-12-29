#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_utils.h"
#include "host.h"

void receive_forward(in_port_t port) {
    int sockfd = init_socket(port);
    byte buffer[BUFLEN];
    struct sockaddr_in src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    int data_len = 0;

    while (1) {
        data_len = recvfrom(sockfd, &buffer, BUFLEN, 0, 
                            (struct sockaddr *)&src_addr, &src_addr_len);
        struct in_addr this_in_addr;
        uint16_t this_port;
        inet_pton(AF_INET, HOST_ADDR, &(this_in_addr.s_addr));
        this_port = htons(port);

        int is_confirm = confirm_dest(buffer, this_in_addr.s_addr, this_port);

        if (is_confirm) {
            uint32_t src_ip_addr, dest_ip_addr;
            uint16_t src_port_ns, dest_port_ns;
            int msg_len;
            char *msg;

            get_packet_data(buffer, &src_ip_addr, &src_port_ns,
                            &dest_ip_addr, &dest_port_ns, &msg_len, &msg);

            display_data(src_ip_addr, src_port_ns, 
                         dest_ip_addr, dest_port_ns, msg_len, msg);
            free(msg);
        } else {
            /*Forward*/
            struct in_addr next_in_addr;
            uint16_t next_port;

            get_cut_next_path(buffer, &data_len, &(next_in_addr.s_addr), &next_port);
            send_data(sockfd, inet_ntoa(next_in_addr), ntohs(next_port), buffer, data_len);

            printf("next: %s:%d\n", inet_ntoa(next_in_addr), ntohs(next_port));
        }  
    }
}

int confirm_dest(byte *packet, uint32_t host_ip_addr, uint16_t host_port_ns) {
    uint32_t dest_ip_addr;
    uint16_t dest_port_ns;

    memcpy(&dest_ip_addr, packet + 6, 4);
    memcpy(&dest_port_ns, packet + 10, 2);

    return host_ip_addr == dest_ip_addr && host_port_ns == dest_port_ns;
}

void get_cut_next_path(byte *packet, int *data_len,
                       uint32_t *next_ip_addr, uint16_t *next_port) {
    *data_len -= 6;
    int offset = *data_len;
    memcpy(next_ip_addr, packet + offset, 4);
    memcpy(next_port, packet + offset + 4, 2);
}

void get_packet_data(byte *packet, uint32_t *src_ip_addr_p, uint16_t *src_port_ns_p,
                     uint32_t *dest_ip_addr_p, uint16_t *dest_port_ns_p, 
                     int *msg_len_p, char **msg_p) {
    int offset = 0;

    memcpy(src_ip_addr_p, packet + offset, 4);
    offset += 4;
    memcpy(src_port_ns_p, packet + offset, 2);
    offset += 2;

    memcpy(dest_ip_addr_p, packet + offset, 4);
    offset += 4;
    memcpy(dest_port_ns_p, packet + offset, 2);
    offset += 2;

    memcpy(msg_len_p, packet + offset, 4);
    offset += 4;

    *msg_p = malloc(*msg_len_p);
    memcpy(*msg_p, packet + offset, *msg_len_p);
}

void display_data(uint32_t src_ip_addr, uint16_t src_port_ns,
                  uint32_t dest_ip_addr, uint16_t dest_port_ns,
                  int msg_len, const char *msg) {
    struct in_addr display_ip;
    in_port_t display_port;

    display_ip.s_addr = src_ip_addr;
    display_port = ntohs(src_port_ns);
    printf("src: %s:%d\n", inet_ntoa(display_ip), display_port);

    display_ip.s_addr = dest_ip_addr;
    display_port = ntohs(dest_port_ns);
    printf("dst: %s:%d\n", inet_ntoa(display_ip), display_port);

    printf("data-len: %d\n", msg_len);
    printf("data: %s\n\n", msg);
}

int main(int argc, char *argv[]) {
    int port = atoi(argv[1]);
    printf("listen on port: %d\n", port);
    receive_forward(port);

    return 0;
}