#include <stdio.h>


int sum(int n) {
    
    if (n == 1) {
        return 1;
    }
    
    
    return n + sum(n - 1);
}

int main() {
    int n;
    
    
    if (scanf("%d", &n) == 1) {
        
        printf("%d", sum(n));
    }
    
    return 0;
}
