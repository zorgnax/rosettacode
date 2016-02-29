#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;
int **graph;
int *path;
int *set;

/* finds hamiltonian paths in the graph if there are any */
void hamiltonian (int i) {
    int j;
    if (i == n) {
        printf("hamiltonian path: ");
        for (j = 0; j < n; j++) {
            printf("%3d", path[j]);
        }
        printf("\n");
        return;
    }
    for (j = 0; j < n; j++) {
        if (!set[j]) {
            if (!i || graph[path[i - 1]][j]) {
                set[j] = 1;
                path[i] = j;
                hamiltonian(i + 1);
                set[j] = 0;
            }
        }
    }
}

void create_graph (int n_) {
    int i, j;
    srand(time(NULL));
    n = n_;
    graph = malloc(n * sizeof (int *));
    for (i = 0; i < n; i++) {
        graph[i] = malloc(n * sizeof (int));
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            }
            else if (j > i) {
                graph[i][j] = rand() % 2;
            }
            else {
                graph[i][j] = graph[j][i];
            }
        }
    }
}

void print_graph () {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%3d", graph[i][j]);
        }
        printf("\n");
    }
}

int main () {
    create_graph(5);
    print_graph();
    path = malloc(n * sizeof (int));
    set = calloc(n, sizeof (int));
    hamiltonian(0);
    return 0;
}

