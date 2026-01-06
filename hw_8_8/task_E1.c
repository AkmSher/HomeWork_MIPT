#include <stdio.h>

int main() {
    int array[5];
    int sum = 0;
    
    for (int i = 0; i < 5; i++) {
        scanf("%d", &array[i]);
        sum += array[i];
    }
    
    double average = (double)sum / 5;
    printf("%.3f", average);
    
    return 0;
}
