#include <stdio.h>
#include <stdlib.h>

// longest common subsequence

#define MAX(a, b) (a > b ? a : b)

int lcs (char *a, int n, char *b, int m, char **s) {
    int i, j, k, t;
    int *z = calloc((n + 1) * (m + 1), sizeof (int));
    int **c = calloc((n + 1), sizeof (int *));
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
    t = c[n][m];
    *s = malloc(t);
    for (i = n, j = m, k = t - 1; k >= 0;) {
        if (a[i - 1] == b[j - 1])
            (*s)[k] = a[i - 1], i--, j--, k--;
        else if (c[i][j - 1] > c[i - 1][j])
            j--;
        else
            i--;
    }
    free(c);
    free(z);
    return t;
}

int main () {
    char a[] = "thisisatest";
    char b[] = "testing123testing";
    int n = sizeof a - 1;
    int m = sizeof b - 1;
    char *s = NULL;
    int t = lcs(a, n, b, m, &s);
    printf("%.*s\n", t, s);
    return 0;
}

