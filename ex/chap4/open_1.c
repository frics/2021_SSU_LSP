#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>


int main(){
	char *fname = "ssu_test.txt";
	int fd;

	if((fd = open(fname, O_RDONLY)) < 0){
		fprintf(stderr, "open erro for %s\n", fname);
		exit(1);
	} else{
		printf("Success!\nFilename : %s\nDescriptor : %d", fname, fd);
	}
	return 0;
}
