#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void signal_handler(int signo){
    printf("sig_handler\n");
}
int main(void){
    struct sigaction sig_act;
    sigset_t set;


    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = signal_handler;
    sigaction(SIGUSR1, &sig_act, NULL);
    printf("before first kill\n");
    raise(SIGUSR1);

    printf("after first kil\n");
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_SETMASK, &set, NULL);
    //두번째부터는 블럭되어 실행되지 않는것을 확인할 수 있다.
    printf("before second kill\n");
    raise(SIGUSR1);
    printf("after second kill\n");
    exit(0);
}