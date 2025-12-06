#include<stdio.h>
int power(int, int);
int main(){
	int a,b;
	scanf("%d%d",&a,&b);
	int res = power(a, b);
	printf("%d",res);
	return 0;
}
int power(int n, int p)
 {
    if(p == 0) return 1;
    if(p < 0) return power ( 1 / n, -p);
    if(p % 2) return n * power (n, p - 1);
    return power(n * n, p / 2);
 }
