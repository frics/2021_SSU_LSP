#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

char glob_str[] = "write to standard output\n";
int glob_var = 10;

int main(){
    pid_t pid;
    int loc_val;

    loc_val = 100;

    if(write(STDOUT_FILENO, glob_str, sizeof(glob_str)-1) != sizeof(glob_str) -1){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    printf("before fork\n");
    if((pid = fork()) < 0){
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if(pid == 0){
        glob_var++;
        loc_val++;
    }else
        sleep(3);
    printf("pid = %d, glob_val = %d, loc_val = %d\n", getpid(), glob_var, loc_val);
    exit(0);
        
    
}