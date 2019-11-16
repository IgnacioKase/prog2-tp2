#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100000

int main(){
	printf("hola\n");
	int a[5000];
	for (int i = 0; i < MAX; i++)
	{
		a[i] = 10;
		printf("%d", a[i]);
	}
	printf("chau\n");
	return 0;
}
