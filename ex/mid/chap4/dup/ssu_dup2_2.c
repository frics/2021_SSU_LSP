#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#define BUFFER_SIZE 1024


int main(){
	char buf[BUFFER_SIZE];
    char *fname = "../ssu_test.txt";
    int fd;
	int length;

    if((fd = open(fname, O_RDONLY, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }
	if(dup2(1, 4) != 4){
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}

	while(1){
		//파일을 읽어서 fd 4로 내보냄
		length = read(fd, buf, BUFFER_SIZE);
		if(length <= 0)
			break;
		//dup2로 stdout를 4번에 복사했기 때문에
		//write 4하면 터미널에 출력이 된다. 
		write(4, buf, length);
	}
    exit(0);
}