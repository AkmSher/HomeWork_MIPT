#include <stdio.h>
	int main(void){
		char a,b,c;
		scanf("%c%c%c",&a,&b,&c);
		int div=(a-'0')*(b-'0')*(c-'0');
		printf("%d",div);
		return 0;
	}
