#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


void check_pending(int signo, char *signame){
    sigset_t set;
    if(sigpending(&set) != 0)
        printf("sigpending error\n");
    else if(sigismember(&set, signo))
        printf("a %s signal is pending\n", signame);
    else
        printf("%s signal is not pending\n", signame);
}

void signal_handler(int signo){
    printf("in signal handler func\n");
}
int main(void){
    struct sigaction sig_act; 
    sigset_t set;

    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = signal_handler;

    if(sigaction(SIGUSR1, &sig_act, NULL) != 0){
        fprintf(stderr, "sigaction() error\n");
        exit(1);
    }else{
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);

        sigprocmask(SIG_SETMASK, &set, NULL);

        printf("SIGUSR1 signal is now blocked\n");
        kill(getpid(), SIGUSR1);
        printf("after kill\n");

        check_pending(SIGUSR1, "SIGUSR1");
        //blocking이 풀리면 핸들러 작동
        //check pending하면 더이상 block되지 않음을 확인할 수 있다.
        sigemptyset(&set);
        sigprocmask(SIG_SETMASK, &set, NULL);

        printf("unblock SIGUSR1\n");
        check_pending(SIGUSR1, "SIGUSR1");

        exit(1);
    }
}