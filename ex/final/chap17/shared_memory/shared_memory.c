#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>


#define SHARED_MEM_SIZE 1024

int main(int argc, char *argv[]){
    key_t key;
    char *data;
    int shared_memory_id;

    if(argc < 2){
        fprintf(stderr, "ftok error\n");
        exit(1);
    }

    if((key = ftok("key", 'B')) == -1){
        fprintf(stderr, "ftok error\n");
        exit(1);
    }

    if((shared_memory_id = shmget(key, SHARED_MEM_SIZE, 0644 | IPC_CREAT)) == -1){
        fprintf(stderr, "shmget error\n");
        exit(1);
    }
    if((data = shmat(shared_memory_id, NULL, 0)) == (char *)(-1)){
        fprintf(stderr, "shmat error\n");
        exit(1);
    }

    if(argc == 2){
        printf("writing to segment : %s\n", argv[1]);
        strncpy(data, argv[1], SHARED_MEM_SIZE);
    }else
        printf("segment contains %s\n", data);
    
    shmctl(shared_memory_id, IPC_RMID, NULL);
    exit(0);
}