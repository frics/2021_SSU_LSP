#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

int shared_value;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *loop1(void *arg){
    
    for(int i=0; i<10; i++){
        pthread_mutex_lock(&mutex);
        printf("loop1 : %d\n", shared_value);
        shared_value++;
        pthread_mutex_unlock(&mutex);
        sleep(1);

    }
    return NULL;
}
void *loop2(void *arg){
    
    for(int i=0; i<10; i++){
        pthread_mutex_lock(&mutex);
        printf("loop2 : %d\n", shared_value);
        shared_value++;
        pthread_mutex_unlock(&mutex);
        sleep(1);

    }
    return NULL;
}

int main(void){
    pthread_t tid1, tid2;
    int status;

    shared_value = 0;

    pthread_create(&tid1, NULL, loop1, NULL);
    pthread_create(&tid2, NULL, loop2, NULL);

    pthread_join(tid1, (void *)&status);
    pthread_join(tid2, (void *)&status);

    status = pthread_mutex_destroy(&mutex);
    printf("code : %d\n", status);
    exit(0);    
}