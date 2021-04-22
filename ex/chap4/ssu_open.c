#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>


int main(){
	char *fname = "ssu_test.txt";
	int fd;
	//읽기 모드로 open()
	if((fd = open(fname, O_RDONLY)) < 0){
		fprintf(stderr, "open erro for %s\n", fname);
		exit(1);
	} else{
		printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);
	}
	return 0;
}
