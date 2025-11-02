#include <stdio.h> 
	int main(void){
		int a, b, c, sum, div;
		scanf("%d %d %d",&a,&b,&c);
		sum=a+b+c;
		div=a*b*c;
		printf("%d+%d+%d=%d\n",a,b,c,sum);
		printf("%d*%d*%d=%d",a,b,c,div);
		return 0;	
	}	 
