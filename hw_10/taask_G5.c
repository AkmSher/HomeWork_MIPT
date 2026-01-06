#include <stdio.h>

int main(void)
{
    FILE *fin, *fout;
    char ch;
    
  
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    // Проверяем успешность открытия файлов
    if (fin != NULL && fout != NULL)
    {
        // Читаем символы до конца файла
        while ((ch = fgetc(fin)) != EOF)
        {
            // Проверяем и заменяем символы
            if (ch == 'a')
            {
                ch = 'b';
            }
            else if (ch == 'b')
            {
                ch = 'a';
            }
            else if (ch == 'A')
            {
                ch = 'B';
            }
            else if (ch == 'B')
            {
                ch = 'A';
            }
        
            
            // Записываем символ в выходной файл
            fputc(ch, fout);
        }
    }
    
 
    if (fin != NULL) fclose(fin);
    if (fout != NULL) fclose(fout);
    
    return 0;
}
