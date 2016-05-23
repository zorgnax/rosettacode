#include <stdio.h>

// euclidean algorithm
int gcd (int a, int b) {
    while (b) {
        int t = a;
        a = b;
        b = t % b;
    }
    return a;
}

int main () {
    int a = 18, b = 84;
    int g = gcd(a, b);
    printf("gcd(%d, %d) = %d\n", a, b, g);
}
