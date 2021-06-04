#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


int main(int argc, char *argv[]){
    int count = 0;
    char *args[] = {"date", NULL};
    
    if(argc > 1)
        count = atoi(argv[1]);
   
    while(count--){
        switch(fork()){
            case 1:
                fprintf(stderr, "fork failed\n");
                exit(1);
            case 0:
                execvp(args[0], args);
            default:
                sleep(1);
                if(count <= 5 && count > 0)
                    printf("강제 종료 %d초 전\n", count);
                break;
        }
        
        if(count == 0)
            raise(SIGKILL);
    }

    
}