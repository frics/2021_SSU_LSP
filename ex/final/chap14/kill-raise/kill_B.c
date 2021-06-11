#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main(int argc, char *argv[]){
    while(1){
        printf("\nTest %d", getpid());
        sleep(2);
    }
    exit(0);
}