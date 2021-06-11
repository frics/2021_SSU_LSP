#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


void signal_handler(int signo){
    printf("sigint occured\n");

}
int main(void){
    sigset_t old_set;
    sigset_t new_set;
    signal(SIGINT, signal_handler);
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);
   
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    printf("잠깐 interupt 받아짐\n");
    sigsuspend(&old_set);

    pause();
    exit(0);
}