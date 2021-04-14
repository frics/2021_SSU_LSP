#include<stdio.h>


int main(){
	int a, b;
	int sum=0;
	scanf("%d %d", &a, &b);

	for(int i = a; i<=b; i++){
		int j=0;
		for(j=2; j<i; j++){
			if((i%j)==0)
				break;
		}
		if(i==j){
			printf("%d\n", i);
			sum += i;
		}
	}
	printf("%d\n", sum);
	return 0;
}

