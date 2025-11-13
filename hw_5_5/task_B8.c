#include <stdio.h>
int main(){
	int a;
	int temp1;
	int coincidence=0;
	scanf("%d",&a);
	while(a>0){
		temp1=a%10;
		a=a/10;
		if(temp1==9){
			coincidence++;
		}
		if(coincidence>1){	
			break;
		}
	}
	if(coincidence==1){
		printf("YES");
	}
	else if(coincidence>1||coincidence==0){
		printf("NO");
	}
	return 0;
}
