#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void (*ssu_func)(int);

void signal_handler(int signo){
    printf("sigint occured\n");
    printf("SIGINT back to default\n");
    //다시 원래의 action으로 되돌린다.
    signal(SIGINT, ssu_func);
}
int main(){
    ssu_func = signal(SIGINT, signal_handler);

    while(1){
        printf("process running\n");
        sleep(1);
    }

    exit(0);
}