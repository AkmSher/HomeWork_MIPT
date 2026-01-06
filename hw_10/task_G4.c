#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *fin, *fout;
    char word1[101], word2[101];
    int count1[26] = {0}; // Счетчики для первого слова
    int count2[26] = {0}; // Счетчики для второго слова
    int first = 1; // Флаг для первого символа (для пробелов)
    
   
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    if (fin != NULL && fout != NULL)
    {
       
        fscanf(fin, "%s %s", word1, word2);
        
     
        for (int i = 0; word1[i] != '\0'; i++)
        {
            count1[word1[i] - 'a']++;
        }
        
   
        for (int i = 0; word2[i] != '\0'; i++)
        {
            count2[word2[i] - 'a']++;
        }
        
        // Ищем символы, которые встречаются ровно 1 раз в обоих словах
        for (int i = 0; i < 26; i++)
        {
            if (count1[i] == 1 && count2[i] == 1)
            {
                if (!first)
                {
                    fprintf(fout, " ");
                }
                fprintf(fout, "%c", 'a' + i);
                first = 0;
            }
        }
        
        fprintf(fout, "\n");
    }
    
    // Закрываем файлы
    if (fin != NULL) fclose(fin);
    if (fout != NULL) fclose(fout);
    
    return 0;
}
