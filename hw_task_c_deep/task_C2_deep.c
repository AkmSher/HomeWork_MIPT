#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STACK 10000

long long stack[MAX_STACK];
int top = -1;

void push(long long val) {
    stack[++top] = val;
}

long long pop() {
    return stack[top--];
}

int main() {
    char token[10000];
    
    while (scanf("%s", token) == 1) {
        /* Проверяем, является ли токен числом */
        /* Число: состоит только из цифр (неотрицательные) */
        int isNumber = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isdigit(token[i])) {
                isNumber = 0;
                break;
            }
        }
        
        if (isNumber && strlen(token) > 0) {
            /* Это число — кладём на стек */
            push(atoll(token));
        } else {
            /* Это оператор или точка (конец) */
            char op = token[0];
            
            if (op == '+' || op == '-' || op == '*' || op == '/') {
                long long b = pop(); /* второй операнд */
                long long a = pop(); /* первый операнд */
                long long result = 0;
                
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                
                push(result);
            }
            /* если '.' или другой символ — игнорируем */
        }
    }
    
    printf("%lld\n", stack[top]);
    
    return 0;
}

