#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define LINE_MAX 2048

static void ssu_alarm(int signo){
    printf("alarm is called\n");
}

int main(void){
    char buf[LINE_MAX];
    int n;
    int remain;

    signal(SIGALRM, ssu_alarm);

    alarm(10);
    n = read(STDIN_FILENO, buf, LINE_MAX);

    remain = alarm(0);

    write(STDOUT_FILENO, buf, n);
    printf("%d seconds remain to alarm\n", remain);
    exit(0);
}