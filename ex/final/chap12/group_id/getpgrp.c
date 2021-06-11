#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main(void){
    pid_t pgid;
    pid_t pid;

   
    switch(fork()){
        case 0:
            pid = getpid();
            pgid = getpgrp();
            printf("child  : pid : %d, pgid : %d\n", pid, pgid);
            exit(1);
        default:
            pid = getpid();
            pgid = getpgrp();
            printf("parent : pid : %d, pgid : %d\n", pid, pgid);
            break;
    }
    
    exit(0);
}