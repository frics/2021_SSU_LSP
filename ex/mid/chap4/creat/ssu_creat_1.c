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
        printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);
    }
    exit(1);
}