#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>


int main(){
	char *fname = "../ssu_test2.txt";
	int fd;

	if((fd=creat(fname, 0666))< 0 ){
		printf("creat error for %s\n", fname);
		exit(1);
	}

	printf("1st screen\n");
	dup2(fd,1);
	printf("Second screen\n");
	exit(0);
}
