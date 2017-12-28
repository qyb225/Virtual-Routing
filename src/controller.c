#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "socket_utils.h"
#include "controller.h"

void control(in_port_t ctrl_port) {
    int sockfd = init_socket(ctrl_port);
    byte buffer[BUFLEN];
    struct search_packet s_packet;

    struct Graph *topo = init_controller();
    
    /*recv args*/
    struct sockaddr_in src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    int data_len = 0;
    
    int count = 0;
    while (count < 3) {
        data_len = recvfrom(sockfd, &s_packet, sizeof(struct search_packet),
                            0, (struct sockaddr *)&src_addr, &src_addr_len);
        int path_len = 0;
        path_len = search_path_packet(topo, s_packet.src_ip_addr, s_packet.src_port, 
                                      s_packet.dest_ip_addr, s_packet.dest_port, buffer);

        sendto(sockfd, &buffer, path_len, 0, (struct sockaddr *)&src_addr, src_addr_len);
        ++count;
    }

    destroy_graph(topo);
}

struct Graph *init_controller() {
    int init_val[4] = { 8000, 8001, 8002, 8003 };
    struct Graph *topo = create_graph(4, init_val);
    add_edge(topo, 0, 1, 1);
    add_edge(topo, 0, 2, 3);
    add_edge(topo, 1, 2, 1);
    add_edge(topo, 2, 3, 1);
    add_edge(topo, 1, 3, 2);

    return topo;
}

int search_path_packet(struct Graph *topo, uint32_t src_ip_addr, uint16_t src_port, 
                        uint32_t dest_ip_addr, uint16_t dest_port, byte *path_packet) {
    int src, dest;
    int path[4];
    int v_num;
    for (int i = 0; i < topo->vertex_num; ++i) {
        if (topo->vertex_list[i].val == (int)ntohs(src_port)) {
            src = i;
        }
        if (topo->vertex_list[i].val == (int)ntohs(dest_port)) {
            dest = i;
        }
    }
    dijkstra(topo, src, dest, path, &v_num);
     
    return package(topo, path, v_num, path_packet);
}

int package(struct Graph *topo, int *path, int path_len, byte *path_packet) {
    int i = path_len - 1;
    int packet_len = 0;
    while (i >= 0) {
        uint32_t router_mid_addr;
        inet_pton(AF_INET, "127.0.0.1", &router_mid_addr);
        memcpy(path_packet + packet_len, &router_mid_addr, 4);
        packet_len += 4;

        uint16_t router_mid_port = htons(topo->vertex_list[path[i]].val);
        memcpy(path_packet + packet_len, &router_mid_port, 2);
        packet_len += 2;

        --i;
    }
    return packet_len;
}

int main() {
    control(8020);

    return 0;
}