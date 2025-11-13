#include <stdio.h>
int main(){
	int a;
	int temp;
	int size=0;
	int count=0;
	scanf("%d",&a);
	while(a>0){
		temp=a%10;
		a=a/10;
		size++;
		if((temp%2)==0){
			count++;
		}
	}
	if(count==size){
		printf("YES");
	}
	else{
		printf("NO");
	}
	return 0;
}
