#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int count = 0;
int input = 0;
int t1 = 0, t2 = 1;

void *thread1(void *arg){
    while(1){
        printf("thred1 start runnig\n");
        pthread_mutex_lock(&mutex1);

        if(input < 2)
            pthread_cond_wait(&cond1, &mutex1);

        if(input == count){
            pthread_cond_signal(&cond2);
            break;
        }

        if(count == 0){
            count++;
            printf("Thread 1 : %d\n", t1);
        }else if(count%2 == 0){
            t1 += t2;
            count++;
            printf("Thread1 : %d\n", t1);
        }
        pthread_cond_signal(&cond2);
        printf("thread1 is waiting\n");
        pthread_cond_wait(&cond1, &mutex1);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}
void *thread2(void *arg){
    while(1){
        printf("thread2 start running\n");
        pthread_mutex_lock(&mutex2);

        if(input < 2)
            pthread_cond_wait(&cond2, &mutex2);

        if(input == count){
            pthread_cond_signal(&cond1);
            break;
        }

        if(count == 1){
            count++;
            printf("Thread 2 : %d\n", t2);
        }else if(count%2 == 1){
            t2 += t1;
            count++;
            printf("Thread2 : %d\n", t2);
        }
        pthread_cond_signal(&cond1);
        printf("thread2 is waiting\n");
        pthread_cond_wait(&cond2, &mutex2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

int main(void){
    pthread_t tid1, tid2;
    int status;

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    while(1){
        printf("2개 이상의 개수 입력 : ");
        scanf("%d", &input);

        if(input >= 2){
            pthread_cond_signal(&cond1);
            break;
        }
    }

    pthread_join(tid1, (void *)&status);
    pthread_join(tid2, (void *)&status);

    printf("complete\n");

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    exit(0);
}