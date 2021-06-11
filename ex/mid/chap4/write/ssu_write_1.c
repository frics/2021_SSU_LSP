#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024

int main(void){
    char buf[BUFFER_SIZE];
    int length;

    //fd = 0 표준 입력이다.
    //표준 입력 -> 키보드 입력
    length = read(0, buf, BUFFER_SIZE);
    write(1, buf, length);
    exit(0);
}