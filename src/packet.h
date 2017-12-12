#ifndef PACKET_H
#define PACKET_H

struct search_packet {
    uint32_t src_ip_addr;
    uint16_t src_port;
    uint32_t dest_ip_addr;
    uint16_t dest_port;
};

#endif