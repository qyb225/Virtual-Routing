#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

struct Graph *create_graph(int n_vertex, int *init_val) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->vertex_num = n_vertex;
    graph->edge_num = 0;

    graph->vertex_list = malloc(n_vertex * sizeof(struct AdjList));
    for (int i = 0; i < n_vertex; ++i) {
        graph->vertex_list[i].val = init_val[i];
        graph->vertex_list[i].head = NULL;
    }

    return graph;
}

void add_edge(struct Graph *graph, int v1, int v2, int weight) {
    /*创建v2的邻接表节点并连接到v1邻接表上*/
    struct ListNode *list_node_v2 = malloc(sizeof(struct ListNode));
    list_node_v2->num = v2;
    list_node_v2->weight = weight;
    list_node_v2->next = graph->vertex_list[v1].head;
    graph->vertex_list[v1].head = list_node_v2;

    /*创建v1的邻接表节点并链接到v2邻接表上*/
    struct ListNode *list_node_v1 = malloc(sizeof(struct ListNode));
    list_node_v1->num = v1;
    list_node_v1->weight = weight;
    list_node_v1->next = graph->vertex_list[v2].head;
    graph->vertex_list[v2].head = list_node_v1;
    
    graph->edge_num += 1;
}

void destroy_graph(struct Graph *graph) {
    for (int i = 0; i < graph->vertex_num; ++i) {
        struct ListNode *del_node = graph->vertex_list[i].head;
        while (del_node) {
            struct ListNode *next_del = del_node->next;
            free(del_node);
            del_node = next_del;
        }
    }
    free(graph->vertex_list);
    free(graph);
}

void display_graph(struct Graph *graph) {
    for (int i = 0; i < graph->vertex_num; ++i) {
        printf("vertex[%d](%d): ", i, graph->vertex_list[i].val);
        struct ListNode *node = graph->vertex_list[i].head;
        while (node) {
            printf("%d(%d) -> ", node->num, node->weight);
            node = node->next;
        }
        printf("NULL\n");
    } 
}