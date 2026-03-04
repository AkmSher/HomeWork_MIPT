#include <stdio.h>

int main() {
    unsigned int n;
    scanf("%u", &n);

    int count = 0;

    while (n != 0) {
        count += n & 1;  // Проверяем младший бит
        n >>= 1;         // Сдвигаем вправо на 1
    }

    printf("%d\n", count);

    return 0;
}

