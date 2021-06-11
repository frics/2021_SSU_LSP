#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

static void signal_handler1(int signo);
static void signal_handler2(int signo);

int main(void){
    struct sigaction act_int, act_quit;

    act_int.sa_handler = signal_handler1;
    sigemptyset(&act_int.sa_mask);
    sigaddset(&act_int.sa_mask, SIGQUIT);
    act_int.sa_flags = 0;
    
    sigaction(SIGINT, &act_int, NULL);

    act_quit.sa_handler = signal_handler2;
    sigemptyset(&act_quit.sa_mask);
    sigaddset(&act_quit.sa_mask, SIGINT);
    act_quit.sa_flags = 0;

    sigaction(SIGQUIT, &act_quit, NULL);

    pause();
    raise(SIGQUIT);
    exit(0);
    
}


static void signal_handler1(int signo){
    printf("sig handler of SIGINT : %d\n", signo);
    printf("SIGQUIT signal is blocked : %d\n", signo);
    printf("sleeping 3 sec\n");

    sleep(3);
    printf("SIGINT handler is end\n");

}
static void signal_handler2(int signo){
    printf("sig handler of SIGQUIT : %d\n", signo);
    printf("SIGINT signal is blocked : %d\n", signo);
    printf("sleeping 3 sec\n");

    sleep(3);
    printf("SIGQUIT handler is end\n");

}