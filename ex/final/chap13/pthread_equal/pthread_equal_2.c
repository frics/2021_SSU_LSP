#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void *arg){
    pthread_t tid;

    tid = pthread_self();

    printf("->%u\n", tid);
    return NULL;
}

int main(void){
    pthread_t tid;

    pthread_create(&tid, NULL, thread, NULL);
    printf("%u\n", tid);
    sleep(1);
    pthread_create(&tid, NULL, thread, NULL);
    printf("%u\n", tid);
    sleep(1);
    exit(0);
}
