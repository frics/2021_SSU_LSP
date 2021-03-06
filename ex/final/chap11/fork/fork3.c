#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

static void ssu_charatatime(char *str);

int main(){
    pid_t pid;

    if((pid=fork()) < 0){
        fprintf(stderr, "fork error\n");
        exit(1);
    }else if(pid == 0)
        ssu_charatatime("output from chlid\n");
    else
        ssu_charatatime("output from parant\n");
    exit(0);
}

static void ssu_charatatime(char *str){
    char *ptr;

    int print_char;
    //NON BUF 모드로 실행
    setbuf(stdout, NULL);

    for(ptr=str; (print_char = *ptr++) != 0;){
        putc(print_char, stdout);
        usleep(1000);
    }
}