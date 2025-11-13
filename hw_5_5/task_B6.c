#include <stdio.h>
#include <stdlib.h>
int main(){
	int a,b,c;
	int flag=0;
	int res = scanf("%d",&a);
	if(res!=1){abort();}
	while(a){
		b=a%10;
		a=(a-b)/10;
		c=a%10;
		if(c==b){
			flag=1;
			break;
		}
	}
	if(flag){
		printf("YES\n");
	} 
	else{
		printf("NO\n");
	}
	return 0;
}
