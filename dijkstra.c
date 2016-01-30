#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int vertex;
    int weight;
} edge_t;

typedef struct {
    edge_t **edge;
    int edge_len;
    int edge_size;

    int dist;
    int prev;
    int visited;
} vertex_t;

typedef struct {
    vertex_t **vertex;
    int vertex_len;
    int vertex_size;
} graph_t;

void add_edge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    int max = a > b ? a : b;
    if (g->vertex_size < max + 1) {
        int size = g->vertex_size * 2 > max ? g->vertex_size * 2 : max + 10;
        g->vertex = realloc(g->vertex, size * sizeof (vertex_t *));
        for (int i = g->vertex_size; i < size; i++) {
            g->vertex[i] = NULL;
        }
        g->vertex_size = size;
    }
    if (!g->vertex[a]) {
        g->vertex[a] = calloc(1, sizeof (vertex_t));
        g->vertex_len++;
    }
    if (!g->vertex[b]) {
        g->vertex[b] = calloc(1, sizeof (vertex_t));
        g->vertex_len++;
    }
    vertex_t *v = g->vertex[a];
    if (v->edge_len >= v->edge_size) {
        v->edge_size = v->edge_size * 2 + 1;
        v->edge = realloc(v->edge, v->edge_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edge[v->edge_len++] = e;
}

void print_graph (graph_t *g) {
    int i, j, k;
    for (i = 0, j = 0; j < g->vertex_len; i++) {
        vertex_t *v = g->vertex[i];
        if (!v)
            continue;
        j++;
        for (k = 0; k < v->edge_len; k++) {
            edge_t *e = v->edge[k];
            printf("%c-[%d]->%c\n", 'a' + i, e->weight, 'a' + e->vertex);
        }
    }
}

void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    vertex_t *v, *u;
    edge_t *e;
    a = a - 'a';
    b = b - 'a';
    for (i = 0; i < g->vertex_len; i++) {
        v = g->vertex[i];
        v->dist = INT_MAX;
        v->prev = 0;
        v->visited = 0;
    }
    i = a;
    v = g->vertex[a];
    v->dist = 0;
    while (v) {
        if (i == b)
            break;
        v->visited = 1;
        for (j = 0; j < v->edge_len; j++) {
            e = v->edge[j];
            u = g->vertex[e->vertex];
            if (v->dist + e->weight <= u->dist) {
                u->prev = i;
                u->dist = v->dist + e->weight;
            }
        }
        for (j = 0, v = NULL; j < g->vertex_len; j++) {
            u = g->vertex[j];
            if (!u->visited && (!v || u->dist <= v->dist)) {
                v = u;
                i = j;
            }
        }
    }
}

void print_path (graph_t *g, int a, int i) {
    a = a - 'a';
    vertex_t *v = g->vertex[a];
    if (v->dist == INT_MAX) {
        printf("no path\n");
        return;
    }
    if (!i) {
        printf("%d ", v->dist);
    }
    if (v->dist) {
        print_path(g, 'a' + v->prev, i + 1);
    }
    printf("%c", 'a' + a);
    if (!i) {
        printf("\n");
    }
}

int main () {
    graph_t *g = calloc(1, sizeof (graph_t));
    add_edge(g, 'a', 'b', 7);
    add_edge(g, 'a', 'c', 9);
    add_edge(g, 'a', 'f', 14);
    add_edge(g, 'b', 'c', 10);
    add_edge(g, 'b', 'd', 15);
    add_edge(g, 'c', 'd', 11);
    add_edge(g, 'c', 'f', 2);
    add_edge(g, 'd', 'e', 6);
    add_edge(g, 'e', 'f', 9);
    print_graph(g);
    dijkstra(g, 'a', 'e');
    print_path(g, 'e', 0);
    return 0;
}

