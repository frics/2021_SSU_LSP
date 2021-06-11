#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>

#define NPROC 50000

void do_nothing(){
    int i;
    i=0;
}

int main(int argc, char *argv[]){
    int pid, j, status;

    struct timeval start, end;
    double delay_time;

    gettimeofday(&start, NULL);

    for(j = 0; j<NPROC; j++){
        if((pid = fork()) < 0 ){
            printf("fork fail with error code = %d\n", pid);
            exit(0);
        }else if(pid == 0){
            do_nothing();
            exit(0);
        }else{
            waitpid(pid, &status, 0);
        }
    }
    gettimeofday(&end, NULL);

    delay_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) /1000000);
    printf("%f seconds\n", delay_time);
    
}