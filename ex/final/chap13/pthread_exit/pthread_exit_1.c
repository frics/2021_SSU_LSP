#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_NUM 5

void *printhello(void *arg){
    int thread_index = *((int *) arg);

    printf("Hello world[%d]\n", thread_index);
    pthread_exit(NULL);
    return NULL;
}

int main(void){
    pthread_t tid[THREAD_NUM];
    int i;

    for(i=0; i<THREAD_NUM; i++){
        printf("in main creating thread %d\n", i);

        if(pthread_create(&tid[i], NULL, printhello, (void *)&i) != 0){
            fprintf(stderr, "thread create error\n");
            exit(1);
        }
    }

    pthread_exit(NULL);
    exit(0);
}