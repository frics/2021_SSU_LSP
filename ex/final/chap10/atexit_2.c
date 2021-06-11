#include<stdio.h>
#include<stdlib.h>


static void ssu_exit1(void);
static void ssu_exit2(void);

int main(void){
    //exit 실행 시 자동으로 실행될 함수를 등록한다.
    if(atexit(ssu_exit2) != 0){
        fprintf(stderr, "atexit error\n");
        exit(1);
    }
    if(atexit(ssu_exit1) != 0){
        fprintf(stderr, "atexit error\n");
        exit(1);
    }
    if(atexit(ssu_exit1) != 0){
        fprintf(stderr, "atexit error\n");
        exit(1);
    }

    printf("exit 함수 등록 완료(호출은 등록의 역순이다!) \n");
    exit(0);
}

static void ssu_exit1(void){
    printf("ssu_exit 1 handler\n");
}


static void ssu_exit2(void){
    printf("ssu_exit 2 handler\n");
}