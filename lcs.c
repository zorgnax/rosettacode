#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_table (char *a, int n, char *b, int m, int **c) {
    int i, j;
    printf(" ");
    for (j = 0; j < m; j++) {
        printf(" %c", b[j]);
    }
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c", a[i]);
        for (j = 0; j < m; j++) {
            printf(" %d", c[i + 1][j + 1]);
        }
        printf("\n");
    }
}

int lcsstr (char *a, int n, char *b, int m, int **c, char **s) {
    int i, j, k;
    *s = malloc(c[n][m]);
    for (i = n, j = m, k = c[n][m]; k;) {
        if (a[i - 1] == b[j - 1]) {
            (*s)[k - 1] = a[i - 1];
            i--;
            j--;
            k--;
        }
        else if (c[i][j - 1] > c[i - 1][j]) {
            j--;
        }
        else {
            i--;
        }
    }
    return c[n][m];
}

void diff (char *a, int n, char *b, int m, int **c, int i, int j) {
    if (i && j && a[i - 1] == b[j - 1]) {
        diff(a, n, b, m, c, i - 1, j - 1);
        // printf("  %c", a[i - 1]);
    }
    else if (j && (!i || c[i][j - 1] >= c[i - 1][j])) {
        diff(a, n, b, m, c, i, j - 1);
        printf(" +%c", b[j - 1]);
    }
    else if (i && (!j || c[i][j - 1] < c[i - 1][j])) {
        diff(a, n, b, m, c, i - 1, j);
        printf(" -%c", a[i - 1]);
    }
    if (i == n && j == m) {
        printf("\n");
    }
}

typedef struct list_t list_t;
struct list_t {
    char *data;
    list_t *next;
};

list_t *append (list_t *a, list_t *b) {
    if (!a) {
        return b;
    }
    list_t *p = a;
    while (p->next) {
        p = p->next;
    }
    p->next = b;
    return a;
}

void lcsall (char *a, int n, char *b, int m, int **c, int i, int j, list_t **s) {
    list_t *z;
    if (i == 0 || j == 0) {
        z = malloc(sizeof (list_t));
        z->data = malloc(c[n][m]);
        z->next = NULL;
        *s = z;
        return;
    }
    if (a[i - 1] == b[j - 1]) {
        lcsall(a, n, b, m, c, i - 1, j - 1, s);
        z = *s;
        while (z) {
            z->data[c[i][j] - 1] = a[i - 1];
            z = z->next;
        }
        if (c[i - 1][j] == c[i][j]) {
            z = NULL;
            lcsall(a, n, b, m, c, i - 1, j, &z);
            *s = append(*s, z);
        }
        if (c[i][j - 1] == c[i][j]) {
            z = NULL;
            lcsall(a, n, b, m, c, i, j - 1, &z);
            *s = append(*s, z);
        }
        return;
    }
    if (c[i - 1][j] >= c[i][j - 1]) {
        z = NULL;
        lcsall(a, n, b, m, c, i - 1, j, &z);
        *s = append(*s, z);
    }
    if (c[i][j - 1] >= c[i - 1][j]) {
        z = NULL;
        lcsall(a, n, b, m, c, i, j - 1, &z);
        *s = append(*s, z);
    }
}

#define MAX(a, b) (a > b ? a : b)

int lcs (char *a, int n, char *b, int m, char **s, list_t **t) {
    int i, j;
    int *z = calloc((n + 1) * (m + 1), sizeof (int));
    int **c = malloc((n + 1) * sizeof (int *));
    for (i = 0; i <= n; i++) {
        c[i] = &z[i * (m + 1)];
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
            }
            else {
                c[i][j] = MAX(c[i - 1][j], c[i][j - 1]);
            }
        }
    }
    print_table(a, n, b, m, c);
    lcsall(a, n, b, m, c, n, m, t);
    lcsstr(a, n, b, m, c, s);
    diff(a, n, b, m, c, n, m);
    return c[n][m];
}

int main (int argc, char **argv) {
    char *a = argv[1];
    int n = strlen(a);
    char *b = argv[2];
    int m = strlen(b);
    printf("a=\"%s\", n=%d, b=\"%s\", m=%d\n", a, n, b, m);
    char *s = NULL;
    list_t *z = NULL;
    int l = lcs(a, n, b, m, &s, &z);
    while (z) {
        // printf("%.*s\n", l, z->data);
        z = z->next;
    }
    printf("lcs: %.*s\n", l, s);
    return 0;
}

