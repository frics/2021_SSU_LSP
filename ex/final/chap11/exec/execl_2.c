#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
    if(fork() == 0){
        sleep(1);
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);
        fprintf(stderr, "exec error\n");
        exit(1);
    }

    if(fork() == 0){
        sleep(1);
        execl("/bin/echo", "echo", "this is", "message two", (char *)0);
        fprintf(stderr, "exec error\n");
        exit(1);
    }

    if(fork() == 0){
        sleep(1);
        execl("/bin/echo", "echo", "this is", "message three", (char *)0);
        fprintf(stderr, "exec error\n");
        exit(1);
    }

    printf("부모 끝~\n");
    exit(0);
}