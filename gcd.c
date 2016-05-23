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

int lcm (int a, int b) {
    return a * b / gcd(a, b);
}

int main () {
    int a = 18, b = 84;
    printf("gcd(%d, %d) = %d\n", a, b, gcd(a, b));
    printf("lcm(%d, %d) = %d\n", a, b, lcm(a, b));
}
