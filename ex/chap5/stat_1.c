#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char **argv){
	struct stat statbuff;

	if(argc != 2){
		fprintf(stderr, "usage : %s<file>\n", argv[0]);
		exit(1);
	}

	if((stat(argv[1], &statbuff))<0){
		fprintf(stderr, "stat error\n");
		exit(1);
	}

	printf("%s is %ld bytes\n", argv[1], statbuff.st_size);
}
