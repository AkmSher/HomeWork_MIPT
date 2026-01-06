#include <stdio.h>

// Рекурсивная функция для печати чисел
void printNumbers(int n) {
    
    if (n < 1) {
        return;
    }
    
    
    printNumbers(n - 1);
    
    
    printf("%d ", n);
}

int main() {
    int n;
    
   
    if (scanf("%d", &n) == 1) {
        // Вызов функции
        printNumbers(n);
    }
    
    return 0;
}
