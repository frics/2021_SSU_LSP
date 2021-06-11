#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_t glo_tid;

void *thread(void *arg){
    printf("자신의 아이디를 전역 변수에 할당\n");
    glo_tid = pthread_self();
}

int main(void){
    pthread_t loc_tid;


    pthread_create(&loc_tid, NULL, thread, NULL);
    sleep(5);
    if(pthread_equal(loc_tid, glo_tid) == 0){
        printf("it is not same thread\n");
        exit(1);
    }
    printf("it is same thread\n");
    exit(0);
}