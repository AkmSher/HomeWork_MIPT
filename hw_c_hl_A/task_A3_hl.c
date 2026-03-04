#include <stdio.h>

int main() {
    unsigned int n, k;
    scanf("%u %u", &n, &k);
    
    // Создаём маску из K единиц
    // (1 << K) - 1 даёт нам K единиц в младших битах
    unsigned int mask = (1u << k) - 1;
    
    // Применяем маску через побитовое И
    unsigned int result = n & mask;
    
    printf("%u\n", result);
    
    return 0;
}

