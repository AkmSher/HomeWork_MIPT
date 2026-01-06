#include <stdio.h>


void print_num(int num) {
    
    if (num >= 10) {
        print_num(num / 10);
    }
    
    
    printf("%d ", num % 10);
}

int main() {
    int n;

    
    if (scanf("%d", &n) == 1) {
        print_num(n);
    }

    return 0;
}

