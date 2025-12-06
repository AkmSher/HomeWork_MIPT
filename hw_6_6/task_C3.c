#include<stdio.h>
int middle(int, int);
int main(){
	int a, b;
	scanf("%d%d",&a,&b);
	int res = middle(a,b);
	printf("%d\n", res);
	return 0;
}
int middle(int a, int b){
	int m = (a+b)/2;
	return m;
}
