#include <stdio.h>
#include <stdlib.h>
int main(){
	int n,square,cube;
	scanf("%d",&n);
	if(n>100&&n<1){abort();}
	for(int i=1; i<=n; i++){
		square=i*i;
		cube=(i*i)*i;
		printf("%d %d %d\n",i,square,cube);
	}
	return 0;
}
