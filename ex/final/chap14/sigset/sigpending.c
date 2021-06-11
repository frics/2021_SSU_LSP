#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(){
    sigset_t pending_list;
    sigset_t set;

    int count = 0;

    sigfillset(&set);
    sigprocmask(SIG_SETMASK, &set, NULL);
    
    alarm(5);
    while(1){
        printf("count : %d\n", count++);
        sleep(1);

        if(sigpending(&pending_list) == 0){
            if(sigismember(&pending_list, SIGINT)){
                printf("SIGINT is pending\n");
                break;
            }else if(sigismember(&pending_list, SIGALRM)){
                printf("ALARM is pending\n");
                break;
            }
            printf("무한 루프 종료\n");
        }
    }

    exit(0);
}