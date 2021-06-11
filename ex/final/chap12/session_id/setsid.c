#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(void){
    pid_t pid;
    printf("parent pid : %d\n", getpid());
    switch(pid = fork()){
        case 0: 
            while(1){
                printf(" %d runnig\n", getpid());
                sleep(5);
            }
        default :
            sleep(5);
            printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
            setsid();
            printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
            kill(pid, SIGKILL);
            exit(0);
    }
   

}