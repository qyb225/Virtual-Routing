#ifndef CTRLLER_H
#define CTRLLER_H

#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "graph.h"
#include "socket_utils.h"

#define BUFLEN 1024

/*API*/
void control(in_port_t ctrl_port);

/*Utils*/
struct Graph *init_controller();

/*分析 sender 发送过来的包的字段*/
void analysis_ask_packet(byte *s_packet, uint32_t *src_ip_addr_p, uint16_t *src_port_ns_p,
                         uint32_t *dest_ip_addr_p, uint16_t *dest_port_ns_p);

/*Dijkstra 算法得到路径*/
int search_path_packet(struct Graph *topo, uint32_t src_ip_addr, uint16_t src_port, 
                        uint32_t dest_ip_addr, uint16_t dest_port, byte *path_packet);

/*根据路径打包数据*/
int package(struct Graph *topo, int *path, int path_len, byte *path_packet);

#endif