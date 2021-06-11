#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    pid_t pid;
    char character, first, last;
    long i;
    switch(fork()){
        case 1:
            fprintf(stderr, "fork failed\n");
            exit(1);
        case 0:
            first = 'A';
            last = 'Z';
            break;
        default:
            first = 'a';
            last = 'z';
            break;
    }

    for(character=first; character<=last; character++){
        for(i=0; i<=1000000;i++);
        write(1, &character, 1);
    }
    printf("\n");
    exit(0);
}