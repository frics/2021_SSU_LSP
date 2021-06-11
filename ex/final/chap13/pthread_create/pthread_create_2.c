#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>


#define THREAD_NUM 8

struct thread_data{
    int thread_index;
    int sum;
    char *message;
};

void *printhello(void *arg){
    struct thread_data *data = (struct thread_data *)arg;
    printf("Thread %d: %s Sum = %d\n", data->thread_index, data->message, data->sum);
    return NULL;
}

struct thread_data thread_data_array[THREAD_NUM];
char *message[THREAD_NUM];

int main(void){
    pthread_t tid[THREAD_NUM];
    int sum;
    int i;

    sum = 0;

    message[0] = "English";
    message[1] = "French";
    message[2] = "Spanish";
    message[3] = "Klingon";
    message[4] = "German";
    message[5] = "Russian";
    message[6] = "Japan";
    message[7] = "Latin";
    for(i=0; i<THREAD_NUM; i++){
        sum = sum+i;

        thread_data_array[i].thread_index = i;
        thread_data_array[i].sum = sum;
        thread_data_array[i].message = message[i];
        printf("Creating thread %d\n",i);
        pthread_create(&tid[i], NULL, printhello, (void *)&thread_data_array[i]);
    }
    sleep(5);
    exit(0);
}