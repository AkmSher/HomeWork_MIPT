#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *fin, *fout;
    char str[1001];
    char longest[1001] = "";
    int max_length = 0;
    
   
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    if (fin != NULL && fout != NULL)
    {
       
        if (fgets(str, sizeof(str), fin) != NULL)
        {
            // Убираем символ новой строки, если он есть
            int len = strlen(str);
            if (len > 0 && str[len - 1] == '\n')
            {
                str[len - 1] = '\0';
            }
            
            // Разбиваем строку на слова
            char *word = strtok(str, " ");
            
            while (word != NULL)
            {
                int word_len = strlen(word);
                
                // Если текущее слово длиннее предыдущего максимума
                if (word_len > max_length)
                {
                    max_length = word_len;
                    strcpy(longest, word);
                }
                
                // Переходим к следующему слову
                word = strtok(NULL, " ");
            }
            
            // Записываем результат
            fprintf(fout, "%s\n", longest);
        }
    }
    
    // Закрываем файлы
    if (fin != NULL) fclose(fin);
    if (fout != NULL) fclose(fout);
    
    return 0;
}
