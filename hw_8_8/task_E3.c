#include <stdio.h>

int main() {
    int arr[10];

    for (int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }

    int max_val = arr[0];
    int max_idx = 0;
    
    int min_val = arr[0];
    int min_idx = 0;

    
    for (int i = 1; i < 10; i++) {
        // Поиск максимума
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_idx = i;
        }
        
        // Поиск минимума
        if (arr[i] < min_val) {
            min_val = arr[i];
            min_idx = i;
        }
    }

    printf("%d %d %d %d", max_idx + 1, max_val, min_idx + 1, min_val);

    return 0;
}
