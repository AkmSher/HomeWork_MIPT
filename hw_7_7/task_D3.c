#include <stdio.h>

int main() {
    int n;

    
    if (scanf("%d", &n) == 1) {
       
        do {
           
            int digit = n % 10;
            
            
            printf("%d ", digit);
            
            
            n /= 10;
        } while (n > 0);
    }

    return 0;
}
