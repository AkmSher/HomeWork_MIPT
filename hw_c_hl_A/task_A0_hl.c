#include<stdio.h>
int main(){

	 int n;
         scanf("%d", &n);
    
         int max;
         int count = 1;
    
    // Читаем первый элемент как начальный максимум
         scanf("%d", &max);
    
    // Читаем оставшиеся n-1 элементов
        for (int i = 1; i < n; i++) {
        	int x;
        	scanf("%d", &x);
        
        	if (x > max) {
            // Нашли новый максимум — обновляем и сбрасываем счётчик
       	        max = x;
                count = 1;
                } else if (x == max) {
            // Максимум повторяется — увеличиваем счётчик
        	 count++;
       		 }
   	 }
    
   	 printf("%d\n", count);
	 return 0;
}
