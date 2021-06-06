#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


int main(int argc, char *argv[]){
    int time_limit = -1;
    char *args[] = {"date", NULL};
    
    if(argc > 1)
        time_limit = atoi(argv[1]);
   
    while(time_limit--){
        switch(fork()){
            case 1:
                fprintf(stderr, "fork failed\n");
                exit(1);
            case 0:
                exe5cvp(args[0], args);
            default:
                sleep(1);
                if(time_limit <= 5 && time_limit > 0)
                    printf("강제 종료 %d초 전\n", time_limit);
                break;
        }   
        if(time_limit == 0)
            raise(SIGKILL);
    }
}