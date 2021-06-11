#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define BUFFER_SIZE 1024

struct msgbuf{
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

int main(void){
    struct msgbuf buf;

    key_t key;
    int msgq_id;

    key = ftok("key", 'A');

    msgq_id = msgget(key, 0644);

    printf("spock : ready to receive messages, captin\n");

    while(1){
        if(msgrcv(msgq_id, &buf, sizeof(buf.msg_text), 0, 0) == -1){
            fprintf(stderr, "msgrcv : msg q is removed\n");
            break;
        }
        printf("spock: %s\n", buf.msg_text);
    }
    exit(0);
}