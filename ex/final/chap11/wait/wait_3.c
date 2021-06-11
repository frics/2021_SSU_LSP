#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void){
    if(fork() == 0)
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);
    if(fork() == 0)
        execl("/bin/echo", "echo", "this is", "message two", (char *)0);

    printf("parent is waiting for children\n");

    //모든 child proc이 종료될때까지 기다린다 
    while(wait((int*)0) != -1);

    printf("all child terminated\n");
    exit(0);
    
}