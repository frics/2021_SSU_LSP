#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


#define VALUE_DONE 100
#define VALUE_STOP1 3
#define VALUE_STOP2 6

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int glo_val = 0;

void *thread1(void *arg){
    while(1){
            pthread_mutex_lock(&lock);
            pthread_cond_wait(&cond, &lock);
            glo_val++;
            printf("global value thread1 : %d\n", glo_val);
            pthread_mutex_unlock(&lock);

            if(glo_val>= VALUE_DONE)
                break;
    }
    return NULL;
}
void *thread2(void *arg){
    while(1){
        pthread_mutex_lock(&lock);
        if(glo_val < VALUE_STOP1 || glo_val > VALUE_STOP2)
            pthread_cond_signal(&cond);
        else{
            glo_val++;
            printf("global value thread2 : %d\n", glo_val);
        }

        pthread_mutex_unlock(&lock);

        if(glo_val >= VALUE_DONE)
            break;  
    }
    return NULL;
}

int main(void){
    pthread_t tid1, tid2;
    int status;

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    pthread_join(tid1, (void *)&status);
    pthread_join(tid2, (void *)&status);

    printf("complete val : %d\n", glo_val);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    exit(0);
}