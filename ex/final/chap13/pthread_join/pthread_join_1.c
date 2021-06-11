#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


void *thread(void *arg){
    int thread_index = *((int *) arg);
    for(int i=0; i<5; i++){
        printf("%d : %d\n", thread_index, i);
        sleep(1);
    }
    return NULL;
}

int main(void){
    pthread_t tid1, tid2;

    int thread1 = 1;
    int thread2 = 2;
    int status;

    pthread_create(&tid1, NULL, thread, (void *)&thread1);
    pthread_create(&tid2, NULL, thread, (void *)&thread2);

    pthread_join(tid1, (void *)&status);
    pthread_join(tid2, (void *)&status);

    exit(0);
}