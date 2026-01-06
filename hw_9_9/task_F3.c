#include <stdio.h>

int main() {
    
    long long counts[10] = {0}; 
    int c;

   
    while ((c = getchar()) != EOF) {
        
        if (c >= '0' && c <= '9') {
            
            counts[c - '0']++;
        }
    }

   
    for (int i = 0; i < 10; i++) {
        if (counts[i] > 0) {
            printf("%d %lld\n", i, counts[i]);
        }
    }

    return 0;
}
