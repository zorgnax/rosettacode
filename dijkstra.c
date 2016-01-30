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
    int vindex;
    int hindex;
} vertex_t;

typedef struct {
    vertex_t **vertex;
    int vertex_len;
    int vertex_size;
} graph_t;

typedef struct {
    vertex_t **vertex;
    int len;
    int size;
} heap_t;

void add_vertex (graph_t *g, int i) {
    if (g->vertex_size < i + 1) {
        int size = g->vertex_size * 2 > i ? g->vertex_size * 2 : i + 4;
        g->vertex = realloc(g->vertex, size * sizeof (vertex_t *));
        for (int j = g->vertex_size; j < size; j++) {
            g->vertex[j] = NULL;
        }
        g->vertex_size = size;
    }
    if (!g->vertex[i]) {
        g->vertex[i] = calloc(1, sizeof (vertex_t));
        g->vertex[i]->vindex = i;
        g->vertex_len++;
    }
}

void add_edge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
    vertex_t *v = g->vertex[a];
    if (v->edge_len >= v->edge_size) {
        v->edge_size = v->edge_size ? v->edge_size * 2 : 4;
        v->edge = realloc(v->edge, v->edge_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edge[v->edge_len++] = e;
}

void push_heap (heap_t *h, vertex_t *v) {
    int i, j;
    if (v->hindex) {
        i = v->hindex;
    }
    else {
        if (h->len + 1 >= h->size) {
            h->size = h->size ? h->size * 2 : 4;
            h->vertex = realloc(h->vertex, h->size * sizeof (vertex_t *));
        }
        h->len++;
        i = h->len;
    }
    j = i / 2;
    while (i > 1 && h->vertex[j]->dist > v->dist) {
        h->vertex[i] = h->vertex[j];
        h->vertex[i]->hindex = i;
        i = j;
        j = j / 2;
    }
    h->vertex[i] = v;
    v->hindex = i;
}

vertex_t *pop_heap (heap_t *h) {
    if (!h->len) {
        return NULL;
    }
    vertex_t *v = h->vertex[1];
    h->vertex[1] = h->vertex[h->len];
    h->len--;
    int i = 1;
    while (1) {
        int k = i;
        int j = 2 * i;
        if (j <= h->len && h->vertex[j]->dist < h->vertex[k]->dist) {
            k = j;
        }
        if (j + 1 <= h->len && h->vertex[j + 1]->dist < h->vertex[k]->dist) {
            k = j + 1;
        }
        if (k == i) {
            break;
        }
        h->vertex[i] = h->vertex[k];
        h->vertex[i]->hindex = i;
        i = k;
    }
    h->vertex[i] = h->vertex[h->len + 1];
    h->vertex[i]->hindex = i;
    return v;
}

void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    a = a - 'a';
    b = b - 'a';
    for (i = 0; i < g->vertex_len; i++) {
        vertex_t *v = g->vertex[i];
        v->dist = INT_MAX;
        v->prev = 0;
        v->visited = 0;
    }
    g->vertex[a]->dist = 0;
    heap_t *h = calloc(1, sizeof (heap_t));
    push_heap(h, g->vertex[a]);
    while (1) {
        vertex_t *v = pop_heap(h);
        if (!v || v->vindex == b) {
            break;
        }
        v->visited = 1;
        for (j = 0; j < v->edge_len; j++) {
            edge_t *e = v->edge[j];
            vertex_t *u = g->vertex[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->prev = v->vindex;
                u->dist = v->dist + e->weight;
                push_heap(h, u);
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
    dijkstra(g, 'a', 'e');
    print_path(g, 'e', 0);
    return 0;
}

