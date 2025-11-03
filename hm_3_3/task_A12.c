#include <stdio.h>
	int main(void){
		char a,b,c;
		scanf("%c%c%c",&a,&b,&c);
		int sum=(a-'0')+(b-'0')+(c-'0');
		printf("%d",sum);
		return 0;
	}
