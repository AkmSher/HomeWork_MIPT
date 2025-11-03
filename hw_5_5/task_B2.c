#include <stdio.h>
#include <stdlib.h>
int main(){
	int a,b,square;
	scanf("%d%d",&a,&b);
	if(a<0){a=-a;}
	if(b<0){b=-b;}
	if(a>100||b>100){abort();}
	for(int i=a; i<=b; i++){
		square=i*i;
		printf("%d ",square);
	}
	return 0;
}
