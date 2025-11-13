#include <stdio.h>
int main(){
	int a;
	int last=0;
	int next=0;
	int size=0;
	int match=0;
	scanf("%d",&a);
	while(a>0){
		last=a%10;
		a=a/10;
		next=a%10;
		size++;
		if(last>next){
			match++;
		}

	}
	if(size==match){
		printf("YES");
	}
	else{printf("NO");}
	return 0;
}
