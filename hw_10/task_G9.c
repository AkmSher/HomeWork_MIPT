#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *fin, *fout;
    char str[1001];
    char result[1001];
    int used[256] = {0}; // Массив для отслеживания использованных символов
    int result_len = 0;
    
 
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    if (fin != NULL && fout != NULL)
    {
    
        if (fgets(str, sizeof(str), fin) != NULL)
        {
            int len = strlen(str);
            
            // Убираем символ новой строки, если он есть
            if (len > 0 && str[len - 1] == '\n')
            {
                str[len - 1] = '\0';
                len--;
            }
            
         
            for (int i = 0; i < len; i++)
            {
                char c = str[i];
                
               
                if (c == ' ')
                {
                    continue;
                }
                
                // Если символ еще не использовался
                if (!used[(unsigned char)c])
                {
                    result[result_len++] = c;
                    used[(unsigned char)c] = 1; // Отмечаем как использованный
                }
            }
            
            // Завершаем строку
            result[result_len] = '\0';
            
            // Записываем результат
            fprintf(fout, "%s\n", result);
        }
    }
    
 
    if (fin != NULL) fclose(fin);
    if (fout != NULL) fclose(fout);
    
    return 0;
}
