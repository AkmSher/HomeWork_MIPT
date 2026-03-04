#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array;           
    uint32_t count0 : 8;      
    uint32_t count1 : 8;      
};

void array2struct(int arr[], struct pack_array *pa) {
    pa->array  = 0;
    pa->count0 = 0;
    pa->count1 = 0;

    for (int i = 0; i < 32; i++) {
        if (arr[i] == 1) {
            
            pa->array |= (1u << (31 - i));
            pa->count1++;
        } else {
            pa->count0++;
        }
    }
}

int main(void) {
    int arr[32];

   
    for (int i = 0; i < 32; i++) {
        scanf("%d", &arr[i]);
    }

    struct pack_array pa;
    array2struct(arr, &pa);

   
    printf("%u %u %u\n", pa.array, pa.count0, pa.count1);

    return 0;
}

