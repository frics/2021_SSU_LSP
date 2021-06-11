#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int count = 0;
void signal_handler(int signo){
    printf("alarm %d\n", count++);
    alarm(1);
}

int  main(void){
    signal(SIGALRM, signal_handler);
    alarm(1);

    while(1);
    exit(0);
}