#include <stdlib.h>

void sort_even_odd(int n, int a[]) {
   
    int *temp = (int*)malloc(n * sizeof(int));
    if (temp == NULL) return; 

    int k = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            temp[k] = a[i];
            k++;
        }
    }

    for (i = 0; i < n; i++) {
        if (a[i] % 2 != 0) {
            temp[k] = a[i];
            k++;
        }
    }

    for (i = 0; i < n; i++) {
        a[i] = temp[i];
    }

    free(temp);
}
