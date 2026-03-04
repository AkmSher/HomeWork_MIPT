#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    char c;
    // Читаем символы до точки
    while (1) {
        scanf("%c", &c);
        
        if (c == '.') {
            // Конец сообщения
            printf(".");
            break;
        } else if (c >= 'a' && c <= 'z') {
            // Строчная буква: алфавит от 'a' до 'z', K = 26
            int num = c - 'a';              // Номер буквы от 0 до 25
            int coded = (num + n) % 26;     // Сдвиг по Цезарю
            printf("%c", 'a' + coded);
        } else if (c >= 'A' && c <= 'Z') {
            // Заглавная буква: алфавит от 'A' до 'Z', K = 26
            int num = c - 'A';              // Номер буквы от 0 до 25
            int coded = (num + n) % 26;     // Сдвиг по Цезарю
            printf("%c", 'A' + coded);
        } else if (c == ' ') {
            // Пробел выводим без изменений
            printf(" ");
        }
    }
    
    printf("\n");
    
    return 0;
}

