#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void signal_handler1(int signo){
    printf("SIGINT occured\n");
}

void signal_handler2(int signo){
    printf("SIGUSR1 occured\n");
}

int main(){
    signal(SIGINT, signal_handler1);
    signal(SIGUSR1, signal_handler2);

    raise(SIGINT);
    raise(SIGUSR1);
    printf("main return\n");
    exit(1);
}