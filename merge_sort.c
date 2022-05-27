#include <stdio.h>
#include <stdlib.h>

void merge (int *a, int *b, int n, int m) {
    int i, j, k;
    for (i = 0, j = m, k = 0; k < n; k++) {
        b[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) {
        a[i] = b[i];
    }
}

void merge_sort2 (int *a, int *b, int n) {
    if (n < 2)
        return;
    int m = n / 2;
    merge_sort2(a, b, m);
    merge_sort2(a + m, b, n - m);
    merge(a, b, n, m);
}

void merge_sort (int *a, int n) {
    int *b = malloc(n * sizeof(int));
    merge_sort2(a, b, n);
    free(b);
}

int main () {
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int n = sizeof a / sizeof a[0];
    int i;
    for (i = 0; i < n; i++)
        printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
    merge_sort(a, n);
    for (i = 0; i < n; i++)
        printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
    return 0;
}
