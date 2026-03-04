#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    unsigned int result = 0;
    unsigned int x;

    for (int i = 0; i < n; i++) {
        scanf("%u", &x);
        result ^= x;  
    }

    printf("%u\n", result);

    return 0;
}

