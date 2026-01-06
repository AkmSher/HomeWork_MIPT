#include <stdio.h>

void to_binary(int n) {
    if (n >= 2) {

        to_binary(n / 2);
    }

    printf("%d", n % 2);
}

int main() {
    int n;
    
    if (scanf("%d", &n) == 1) {
        to_binary(n);
    }
    
    return 0;
}
