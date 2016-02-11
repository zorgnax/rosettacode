#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

typedef struct {
    int vertex;
    int weight;
} edge_t;

typedef struct {
    edge_t **edges;
    int edges_len;
    int edges_size;
    int hindex;
    int dist;
    int prev;
    int visited;
} vertex_t;

typedef struct {
    vertex_t **vertices;
    int vertices_len;
    int vertices_size;
} graph_t;

typedef struct {
    int *vertices;
    int len;
    int size;
} heap_t;

void add_vertex (graph_t *g, int i) {
    if (g->vertices_size < i + 1) {
        int size = g->vertices_size * 2 > i ? g->vertices_size * 2 : i + 4;
        g->vertices = realloc(g->vertices, size * sizeof (vertex_t *));
        for (int j = g->vertices_size; j < size; j++) {
            g->vertices[j] = NULL;
        }
        g->vertices_size = size;
    }
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (vertex_t));
        g->vertices_len++;
    }
}

void add_edge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
    vertex_t *v = g->vertices[a];
    if (v->edges_len >= v->edges_size) {
        v->edges_size = v->edges_size ? v->edges_size * 2 : 4;
        v->edges = realloc(v->edges, v->edges_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edges[v->edges_len++] = e;
}

void up_heap (heap_t *h, graph_t *g, int i) {
    vertex_t *v = g->vertices[i];
    int j = v->hindex;
    int k = j / 2;
    while (j > 1) {
        vertex_t *u = g->vertices[h->vertices[k]];
        if (u->dist < v->dist)
            break;
        h->vertices[j] = h->vertices[k];
        u->hindex = j;
        j = k;
        k = k / 2;
    }
    h->vertices[j] = i;
    v->hindex = j;
}

void push_heap (heap_t *h, graph_t *g, int i) {
    vertex_t *v = g->vertices[i];
    if (!v->hindex) {
        if (h->len + 1 >= h->size) {
            h->size = h->size ? h->size * 2 : 4;
            h->vertices = realloc(h->vertices, h->size * sizeof (int));
        }
        h->len++;
        v->hindex = h->len;
    }
    up_heap(h, g, i);
}

int min (heap_t *h, graph_t *g, int n, ...) {
    va_list args;
    va_start(args, n);
    int m = va_arg(args, int);
    int i, j;
    for (i = 0; i < n - 1; i++) {
        j = va_arg(args, int);
        if (j > h->len)
            break;
        vertex_t *v = g->vertices[h->vertices[m]];
        vertex_t *u = g->vertices[h->vertices[j]];
        if (u->dist < v->dist)
            m = j;
    }
    va_end(args);
    return m;
}

void down_heap (heap_t *h, graph_t *g) {
    int k = h->vertices[1];
    int i = 1;
    while (1) {
        int j = min(h, g, 3, i, 2 * i, 2 * i + 1);
        if (j == i)
            break;
        h->vertices[i] = h->vertices[j];
        g->vertices[h->vertices[i]]->hindex = i;
        i = j;
    }
    h->vertices[i] = k;
    g->vertices[k]->hindex = i;
}

int pop_heap (heap_t *h, graph_t *g) {
    int i = h->vertices[1];
    h->vertices[1] = h->vertices[h->len];
    h->len--;
    down_heap(h, g);
    return i;
}

void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    a = a - 'a';
    b = b - 'a';
    for (i = 0; i < g->vertices_len; i++) {
        vertex_t *v = g->vertices[i];
        v->dist = INT_MAX;
        v->prev = 0;
        v->visited = 0;
    }
    vertex_t *v = g->vertices[a];
    v->dist = 0;
    heap_t *h = calloc(1, sizeof (heap_t));
    push_heap(h, g, a);
    while (h->len) {
        i = pop_heap(h, g);
        if (i == b)
            break;
        v = g->vertices[i];
        v->visited = 1;
        for (j = 0; j < v->edges_len; j++) {
            edge_t *e = v->edges[j];
            vertex_t *u = g->vertices[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->prev = i;
                u->dist = v->dist + e->weight;
                push_heap(h, g, e->vertex);
            }
        }
    }
}

void print_path (graph_t *g, int i) {
    i = i - 'a';
    vertex_t *v = g->vertices[i], *u = v;
    if (v->dist == INT_MAX) {
        printf("no path\n");
        return;
    }
    int j = 1, k;
    while (u->dist) {
        u = g->vertices[u->prev];
        j++;
    }
    char *a = malloc(j);
    a[j - 1] = 'a' + i;
    for (k = 0, u = v; k < j - 1; k++, u = g->vertices[u->prev]) {
        a[j - k - 2] = 'a' + u->prev;
    }
    printf("%d %.*s\n", v->dist, j, a);
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
    print_path(g, 'e');
    return 0;
}

