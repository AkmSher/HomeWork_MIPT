#include <stdio.h>

int main(void)
{
    FILE *fin, *fout;
    int n;

    // Открываем файлы
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

   
    if (fscanf(fin, "%d", &n) == 1)
    {
      
        for (int i = 0; i < n / 2; i++)
        {
            
            fprintf(fout, "%c", 'A' + i);

            int digit = 2 + (i % 4) * 2;
            fprintf(fout, "%d", digit);
        }
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
