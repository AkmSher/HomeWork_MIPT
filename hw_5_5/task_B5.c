#include <stdio.h>
#include <stdlib.h>
int main(){
	int a;
	int sum=0;
	int res=scanf("%d",&a);
	if(a<0||res!=1){abort();}
	while(a){
		int remainder=a%10;
		sum=sum+remainder;
		a=(a-remainder)/10;
	}
	printf("%d\n",sum);
	return 0;
}
