#include <stdio.h>
#include <string.h>

int main() {
    char s[10005];
    scanf("%s", s);
    int n = strlen(s);
    
    // Множество найденных трёхзначных чисел (100..999)
    int found[1000] = {0};
    int count = 0;
    
    // Перебираем все тройки индексов i < j < k
    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                int d1 = s[i] - '0';
                int d2 = s[j] - '0';
                int d3 = s[k] - '0';
                
                // Трёхзначное число: первая цифра не 0
                if (d1 != 0) {
                    int num = d1 * 100 + d2 * 10 + d3;
                    if (!found[num]) {
                        found[num] = 1;
                        count++;
                    }
                }
            }
        }
    }
    
    printf("%d\n", count);
    return 0;
}

