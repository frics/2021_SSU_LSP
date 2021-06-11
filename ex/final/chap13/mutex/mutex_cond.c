#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


pthread_t tid[5];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int shared_value;
void *thread(void *arg){
    int loc_id = *((int *)arg);
   
    pthread_mutex_lock(&mutex);
    printf("%d is runnig\n", loc_id);
    pthread_cond_wait(&cond, &mutex);
    
    printf("%d thread is waked\n", loc_id);
    shared_value++;
    printf("Thread no.%d, Shared value %d\n", loc_id, shared_value);
    pthread_mutex_unlock(&mutex);
}

int main(void){
    int i, id[5];

    shared_value = 0;

    for(i =0; i<5; i++){
        id[i] = i;
        pthread_create(&tid[i], NULL, thread, (void *)&id[i]);
    }
    sleep(1);

    pthread_cond_broadcast(&cond);
    for(i=0; i<5; i++){
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;

}