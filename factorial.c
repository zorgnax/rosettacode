#include <stdio.h>
#include <stdlib.h>

int factorial (int n) {
    int f = 1;
    for (; n; n--)
        f *= n;
    return f;
}

int main (int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "one argument needed\n");
        return 1;
    }
    int n = atoi(argv[1]);
    printf("%d! = %d\n", n, factorial(n));
    return 0;
}
