#include<stdio.h>
#include<stdlib.h>//exit(1)
#include<unistd.h>
#include<fcntl.h>


int main(void){
    char *fname = "ssu_test.txt";
    int fd;

    if((fd=creat(fname, 0666)) < 0 ){
        fprintf(stderr, "creat error for %s\n", fname);
    }
    else{
        close(fd);
        if((fd = open(fname, O_RDONLY)) < 0){
            printf("Fail\n");
        }else{
            printf("Success!\n<%s>\n", fname);
        }
    }
    exit(1);
}