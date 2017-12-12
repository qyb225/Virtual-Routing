#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "socket_utils.h"

#define BUFLEN 1024

void control();

void control() {
    int sockfd = init_socket(8020);
    byte buffer[BUFLEN];
    struct search_packet s_packet;
    
    /*recv args*/
    struct sockaddr_in src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    int data_len = 0;
    
    while (1) {
        data_len = recvfrom(sockfd, &s_packet, sizeof(struct search_packet), 0, 
            (struct sockaddr *)&src_addr, &src_addr_len);

        // struct in_addr src_in_addr;
        // struct in_addr dest_in_addr;
        // src_in_addr.s_addr = s_packet.src_ip_addr;
        // dest_in_addr.s_addr = s_packet.dest_ip_addr;

        // printf("src: %s:%d\n", inet_ntoa(src_in_addr), ntohs(s_packet.src_port));
        // printf("dst: %s:%d\n\n", inet_ntoa(dest_in_addr), ntohs(s_packet.dest_port));

        /*Main Algorithm...(Todo)*/

        /*Recive path_packet(TEST)*/
        int path_len = 0;
        uint32_t router_mid_addr;
        inet_pton(AF_INET, "127.0.0.1", &router_mid_addr);
        uint16_t router_mid_port = htons(8013);
        memcpy(buffer + path_len, &router_mid_addr, 4);
        path_len += 4;
        memcpy(buffer + path_len, &router_mid_port, 2);
        path_len += 2;

        sendto(sockfd, &buffer, path_len, 0, (struct sockaddr *)&src_addr, src_addr_len);
    }
}

int main() {
    control();

    return 0;
}