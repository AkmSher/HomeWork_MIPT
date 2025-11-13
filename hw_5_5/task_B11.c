#include <stdio.h>
int main(){
	int a;
	int last=0;
	int revers=0;
	scanf("%d",&a);
	while(a>0){
		last=a%10;
		a=a/10;
		revers=revers*10+last;
	}
	printf("%d\n",revers);
	return 0;
}
