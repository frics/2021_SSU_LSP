#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define TABLE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    static struct{
        long offset;
        int length;
    }table[TABLE_SIZE];

    char buf[BUFFER_SIZE];
    int entry;
    int i;
    int length;
    int fd;

    if(argc < 2){
        fprintf(stderr, "usage : %s <file>\n", argv[0]);
    }
    if((fd = open(argv[1], O_RDONLY)) < 0){
        fprintf(stderr, "open error for %s\n", argv[1]);
        exit(1);
    }

    entry = 0;
    offset = 0;
    while(length = read)
}