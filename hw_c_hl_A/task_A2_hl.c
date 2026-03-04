#include <stdio.h>

int main() {
    unsigned int n, k;
    scanf("%u %u", &n, &k);
    
    unsigned int result = (n >> k) | (n << (32 - k));
    
    printf("%u\n", result);
    
    return 0;
}

