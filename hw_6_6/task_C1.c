#include <stdio.h>
void module_num(int a){
	a = a<0 ? -a : a;
	printf("%d\n", a);
}
int main(){
	int b = 0;
	scanf("%d", &b); 
	module_num(b);
	
	return 0;
}
