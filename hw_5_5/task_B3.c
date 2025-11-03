#include <stdio.h>
#include <stdlib.h>
int main(){
	int a,b,square;
	int sum=0;
	scanf("%d%d",&a,&b);
	if(a<0){a=-a;}
	if(b<0){b=-b;}
	if(a>100||b>100||a>b){abort();}
	for(int i=a; i<=b; i++){
		square=i*i;
		sum=sum+square;
	}
	printf("%d\n",sum);
	return 0;
}
