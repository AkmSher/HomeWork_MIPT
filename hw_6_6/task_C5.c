#include<stdio.h>
int sum_n(int);
int main(){
	int n;
	scanf("%d",&n);
	int res = sum_n(n);
	printf("%d\n", res);
	return 0;
}
int sum_n(int n){
	int sum = ((1+n)*n)/2;
	return sum;
}
