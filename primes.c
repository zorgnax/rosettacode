#include <stdio.h>

int isprime (int n) {
    int i;
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    for (i = 2; i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main () {
    int i;
    for (i = 0;; i++) {
        if (isprime(i)) {
            printf("%d\n", i);
        }
    }
    return 0;
}
