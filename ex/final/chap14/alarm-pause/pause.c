#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void my_alarm(int signo){
    printf("alarm...!\n");
}
void my_int_handler(int signo){
    printf("프로그램 종료\n");
    exit(1);
}
int main(void){
    printf("Alarm setting\n");
    signal(SIGALRM, my_alarm);
    signal(SIGINT, my_int_handler);
    alarm(2);

    while(1){
        printf("done\n");
        pause();
        alarm(2);
    }

    exit(0);

}