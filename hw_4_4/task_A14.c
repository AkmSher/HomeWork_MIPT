#include <stdio.h>
	int main(){
		char a,b,c;
		int max;
		scanf("%c%c%c",&a,&b,&c);
		if((a>b)&&(a>c)){
			max=a-'0';
		}
		else if((b>a)&&(b>c)){
			max=b-'0';
		}
		else{
			max=c-'0';
		}
		printf("%d",max);
		return 0;
	}
