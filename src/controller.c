#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_utils.h"
#include "controller.h"

void control(in_port_t ctrl_port) {
    int sockfd = init_socket(ctrl_port);
    byte buffer[BUFLEN];
    byte s_packet[12];

    struct Graph *topo = init_controller();
    
    /*recv args*/
    struct sockaddr_in src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    int data_len = 0;
    
    int count = 0;
    while (count < 3) {
        data_len = recvfrom(sockfd, &s_packet, sizeof(s_packet),
                            0, (struct sockaddr *)&src_addr, &src_addr_len);
        int path_len = 0;
        
        uint32_t src_ip_addr, dest_ip_addr;
        uint16_t src_port_ns, dest_port_ns;
        analysis_ask_packet(s_packet, &src_ip_addr, &src_port_ns,
                            &dest_ip_addr, &dest_port_ns);

        path_len = search_path_packet(topo, src_ip_addr, src_port_ns, 
                                      dest_ip_addr, dest_port_ns, buffer);

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

void analysis_ask_packet(byte *s_packet, uint32_t *src_ip_addr_p, uint16_t *src_port_ns_p,
                         uint32_t *dest_ip_addr_p, uint16_t *dest_port_ns_p) {
    int offset = 0;
    /*src*/
    memcpy(src_ip_addr_p, s_packet + offset, 4);
    offset += 4;
    memcpy(src_port_ns_p, s_packet + offset, 2);
    offset += 2;

    /*dest*/
    memcpy(dest_ip_addr_p, s_packet + offset, 4);
    offset += 4;
    memcpy(dest_port_ns_p, s_packet + offset, 2);
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