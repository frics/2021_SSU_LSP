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

    if((key = ftok("key", 'A')) == -1){
        fprintf(stderr, "ftok error\n");
        exit(1);
    }

    if((msgq_id = msgget(key, 0644 | IPC_CREAT)) == -1){
        fprintf(stderr, "msgget error %s\n", strerror(errno));
        exit(1);
    }

    printf("Enter lines of text, ^D to quit: \n");
    buf.msg_type = 1;

    while(fgets(buf.msg_text, sizeof(buf.msg_text), stdin) != NULL){
        int length = strlen(buf.msg_text);

        if(buf.msg_text[length-1] == '\n')
            buf.msg_text[length-1] = '\0';
        if(msgsnd(msgq_id, &buf, sizeof(buf) - sizeof(long), 0) == -1){
            fprintf(stderr, "msgsnd error\n");
            exit(1);
        }
    }

    if(msgctl(msgq_id, IPC_RMID, NULL) == -1){
        fprintf(stderr, "msgctl error\n");
        exit(1);
    }

    exit(0);
}