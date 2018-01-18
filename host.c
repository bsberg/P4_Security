#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv){
	
	int a = 0; 
	int b = 0;
	printf("This is a super cool program that adds stuff\n");
	printf("Pick a number\n");
	scanf("%d", &a);
	printf("Pick another number\n");
	scanf("%d", &b);
	a = a + b;
	printf("Together they are: %d!\n", a);



}