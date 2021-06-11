#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void *ssu_thread(void *arg){
    pthread_t tid;
    pid_t pid;

    pid = getpid();
    tid = pthread_self();

    printf("New Thread : pid %d tid %u\n", pid, tid);
    return NULL;

}

int main(void){
    pthread_t tid;
    pid_t pid;

    if(pthread_create(&tid, NULL, ssu_thread, NULL) != 0){
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    pid = getpid();
    tid = pthread_self();
    printf("Main Thread : pid %d tid %u\n", pid, tid);
    sleep(1);
    exit(0);
}