#include<stdio.h>
unsigned long long chess_corn(int);
unsigned long long power(unsigned long long, int);
int main(){
	int a;
	scanf("%d",&a);
	unsigned long long res = chess_corn(a);
	printf("%llu\n",res);
	return 0;
}
unsigned long long chess_corn(int b){
	int c = b-1;
	unsigned long long res = power(2,c);
	return res;	
}
unsigned long long power(unsigned long long x, int n){
    	unsigned long long a = 1;
     while(n) {
         if(n % 2) {
             a *= x;
             n--;
         }
         else{  
             x *= x;
             n /= 2;    
         }
     }
     return a;
}
