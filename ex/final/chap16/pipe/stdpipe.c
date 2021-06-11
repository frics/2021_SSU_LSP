#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAXLINE 20

int main(int argc, char *argv[]){
    int n, pid, fd[2];
    char line[MAXLINE];

    pipe(fd);

    if((pid = fork()) == 0 ){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        while(1){
            printf("sibal funcking\n");
            
        }
    }else{
        close(fd[1]);
        //dup2(fd[0], 0);
        printf("String from fucking chlid\n");
        while(( n = read(fd[0], line, MAXLINE)) > 0)
            write(STDOUT_FILENO, line, n);
    }
    exit(0);
}