#include <stdio.h>
int is_prime(int n){
	int count = 0;
	if(n<2){return 1;}
	
	for(int i = 1; i<=n; i++){
			
		if(n%i==0){count++;}

	}
	if(count==2){return 0;}
	
	return 1;
}
int main(){
	int num = 0;
	scanf("%d", &num);
	if(is_prime(num)==0){
		printf("YES\n");
	}else{
		printf("NO\n");
	}
       	return 0;
}
