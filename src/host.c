#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"
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
        /*Confirm*/
        struct in_addr dest_in_addr, this_in_addr;
        uint16_t dest_port, this_port;

        memcpy(&(dest_in_addr.s_addr), buffer + 6, 4);
        memcpy(&dest_port, buffer + 10, 2);

        inet_pton(AF_INET, HOST_ADDR, &(this_in_addr.s_addr));
        this_port = htons(port);

        if (this_in_addr.s_addr == dest_in_addr.s_addr && dest_port == this_port) {
            int offset = 0;
            struct in_addr packet_in_addr;
            uint16_t temp_port;

            memcpy(&(packet_in_addr.s_addr), buffer + offset, 4);
            offset += 4;
            memcpy(&temp_port, buffer + offset, 2);
            offset += 2;

            printf("get data packet!\n");
            printf("src: %s:%d\n", inet_ntoa(packet_in_addr), ntohs(temp_port));

            memcpy(&(packet_in_addr.s_addr), buffer + offset, 4);
            offset += 4;
            memcpy(&temp_port, buffer + offset, 2);
            offset += 2;

            printf("dst: %s:%d\n", inet_ntoa(packet_in_addr), ntohs(temp_port));

            uint32_t temp_msg_len;
            memcpy(&temp_msg_len, buffer + offset, 4);
            offset += 4;

            printf("msg-len: %d\n", temp_msg_len);

            char *temp_msg = malloc(temp_msg_len);
            memcpy(temp_msg, buffer + offset, temp_msg_len);
            offset += temp_msg_len;

            printf("msg: %s\n", temp_msg);
            free(temp_msg);
        } else {
            /*Forward*/
            struct in_addr next_in_addr;
            uint16_t next_port;
            data_len -= 6;
            memcpy(&(next_in_addr.s_addr), buffer + data_len, 4);
            memcpy(&next_port, buffer + data_len + 4, 2);

            printf("next: %s:%d\n", inet_ntoa(next_in_addr), ntohs(next_port));

            send_data(sockfd, inet_ntoa(next_in_addr), ntohs(next_port), buffer, data_len);
        }  
    }
}

int main(int argc, char *argv[]) {
    receive_forward(atoi(argv[0]));

    return 0;
}