#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>


int main(void){
    int fd, state;

    state = seteuid(10000);

    if(state < 0){
        fprintf(stderr, "seteuid error\n");
        exit(1);
    }

    if((fd = open("ssu_test.txt", O_CREAT | O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error(%s)\n",strerror(errno));
        exit(1);
    }
    close(fd);
    exit(0);
}