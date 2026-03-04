#include <stdio.h>

int main() {
    unsigned int n;
    scanf("%u", &n);

    // Маска для старшего байта (биты 24-31)
    // 0xFF000000 = 11111111 00000000 00000000 00000000
    unsigned int mask = 0xFF000000;

    // XOR с маской инвертирует только биты старшего байта
    n = n ^ mask;

    printf("%u\n", n);

    return 0;
}

