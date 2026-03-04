#include <stdio.h>

int main() {
    unsigned int n, k;
    scanf("%u %u", &n, &k);

    // Маска из K единиц
    unsigned int mask = (1u << k) - 1;
    unsigned int max_val = 0;

    // Перебираем все возможные позиции окна шириной K
    for (int i = 0; i <= 32 - (int)k; i++) {
        // Сдвигаем N вправо на i позиций и берём K младших битов
        unsigned int current = (n >> i) & mask;

        if (current > max_val) {
            max_val = current;
        }
    }

    printf("%u\n", max_val);

    return 0;
}

