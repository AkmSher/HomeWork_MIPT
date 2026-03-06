#include <stdio.h>
#include <string.h>

int main() {
    char s1[10001], s2[10002];
    int len1, len2;
    int ans1 = 0, ans2 = 0;
    
    scanf("%s", s1);
    scanf("%s", s2);
    
    len1 = strlen(s1);
    len2 = strlen(s2);
    
    /* 
     * ans1: длина наибольшего префикса s1, являющегося суффиксом s2
     * Префикс s1 длины k: s1[0..k-1]
     * Суффикс s2 длины k: s2[len2-k..len2-1]
     * Проверяем для k от min(len1,len2) до 1
     */
    int maxLen = len1 < len2 ? len1 : len2;
    
    for (int k = maxLen; k >= 1; k--) {
        /* сравниваем s1[0..k-1] с s2[len2-k..len2-1] */
        if (strncmp(s1, s2 + len2 - k, k) == 0) {
            ans1 = k;
            break;
        }
    }
    
    /*
     * ans2: длина наибольшего суффикса s1, являющегося префиксом s2
     * Суффикс s1 длины k: s1[len1-k..len1-1]
     * Префикс s2 длины k: s2[0..k-1]
     * Проверяем для k от min(len1,len2) до 1
     */
    for (int k = maxLen; k >= 1; k--) {
        /* сравниваем s1[len1-k..len1-1] с s2[0..k-1] */
        if (strncmp(s1 + len1 - k, s2, k) == 0) {
            ans2 = k;
            break;
        }
    }
    
    printf("%d %d\n", ans1, ans2);
    
    return 0;
}

