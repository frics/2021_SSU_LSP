#include<stdio.h>
#include<stdlib.h>


void ssu_out(void);

int main(void){
    //exit 실행 시 자동으로 실행될 함수를 등록한다.
    if(atexit(ssu_out)){
        fprintf(stderr, "atexit error\n");
        exit(1);
    }

    exit(0);
}

void ssu_out(void){
    printf("atexit succeeded\n");
}