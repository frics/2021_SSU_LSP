#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void *arg){
    printf("thread start\n");
    sleep(5);
    printf("thread end\n");
    pthread_exit(NULL);
    return NULL;
}

int main(void){
    pthread_t tid;
    
    pthread_create(&tid, NULL, thread, NULL);
    sleep(1);

    printf("스레드 완료 전 main func exit -> thread boom\n");
    printf("bye main\n");
}