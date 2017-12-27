#ifndef GRAPH_H
#define GRAPH_H

struct ListNode {
    int num; /*顶点编号*/
    int weight; /*权重(for edge)*/
    struct ListNode *next; /*下一个顶点*/
};

struct AdjList {
    int val; /*邻接表起始顶点对应数值*/
    struct ListNode *head;
};

struct Graph {
    int vertex_num; /*顶点数目*/
    int edge_num; /*边数目*/
    struct AdjList *vertex_list; /*每个顶点对应的邻接表*/
};

/*创建图*/
struct Graph *create_graph(int n_vertex, int *init_val);

/*添加 v1 和 v2 的无向边*/
void add_edge(struct Graph *graph, int v1, int v2, int weight);

/*销毁图*/
void destroy_graph(struct Graph *graph);

/*输出邻接表 (FOR DEBUGGING)*/
void display_graph(struct Graph *graph);

#endif