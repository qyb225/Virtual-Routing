#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

#define MAX_NUM 1024

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

int dijkstra(struct Graph *graph, int v1, int v2, int *path, int *v_num) {
    /*存放更新到的最短距离*/
    int *dis = malloc(graph->vertex_num * sizeof(int));
    /*已经扩展的集合*/
    int *exp_sets = malloc(graph->vertex_num * sizeof(int));

    for (int i = 0; i < graph->vertex_num; ++i) {
        dis[i] = MAX_NUM;
        exp_sets[i] = 0;
    }
    exp_sets[v1] = 1; /*v1 被扩展*/
    dis[v1] = 0;
    int least_exp = v1;
    int least_exp_dis = 0;
    *v_num = 0;
    while (!exp_sets[v2]) {
        struct ListNode *successor = graph->vertex_list[least_exp].head;
        int next_exp;
        int next_min_dis = MAX_NUM;
        while (successor) {
            if (!exp_sets[successor->num]) {
                if (least_exp_dis + successor->weight < dis[successor->num]) {
                    dis[successor->num] = least_exp_dis + successor->weight;
                }
                if (dis[successor->num] < next_min_dis) {
                    next_min_dis = dis[successor->num];
                    next_exp = successor->num;
                }
            }      
            successor = successor->next;
        }
        least_exp = next_exp;
        least_exp_dis = dis[least_exp];
        exp_sets[least_exp] = 1;
        path[(*v_num)++] = least_exp;
    }
    free(dis);
    free(exp_sets);
    
    return least_exp_dis;
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