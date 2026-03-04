#include <stdio.h>
#include <stdint.h>
#include <string.h>

int extractExp(float f) {
    uint32_t bits;
    

    memcpy(&bits, &f, sizeof(bits));
    

    return (int)((bits >> 23) & 0xFF);
}

int main(void) {
    float f;
    scanf("%f", &f);
    
    printf("%d\n", extractExp(f));
    
    return 0;
}

