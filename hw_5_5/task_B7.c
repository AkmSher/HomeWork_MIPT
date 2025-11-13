#include <stdio.h>
int main(){
	int a;
	int temp;
	int temp1;
	int temp2;
	int flag=0;
	scanf("%d",&a);
	while(a>0){
		temp1=a%10;
		a=a/10;
		temp=a;

		while(a>0){
			temp2=a%10;
			a=a/10;
			if(temp1==temp2){
				printf("YES");
				flag=1;
				break;
			}

		}
		if(flag){break;}
		a=temp;
	}
	if(flag==0){
		printf("NO");
	}
	return 0;
}
