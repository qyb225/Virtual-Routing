#include "socket_utils.h"

void send_data(int sockfd, const char *dest_addr_str,
               in_port_t dest_port, void *data_send, int data_len) {
    
    byte *data = (byte *)data_send;
    
    struct sockaddr_in dest_addr = set_dest_addr(dest_addr_str, dest_port);
    sendto(sockfd, data, data_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

struct sockaddr_in set_dest_addr(const char *dest_addr_str, in_port_t dest_port) {
    struct sockaddr_in dest_addr;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(dest_port);
    //inet_aton(dest_addr_str, &(dest_addr.sin_addr.s_addr));
    inet_pton(AF_INET, dest_addr_str, &(dest_addr.sin_addr.s_addr));
    return dest_addr;
}

int init_socket(in_port_t port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in host_addr;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);

    inet_pton(AF_INET, "127.0.0.1", &(host_addr.sin_addr.s_addr));

    bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr));

    return sockfd;
}