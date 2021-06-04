#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
#include<signal.h>

int main(int argc, char *argv[]){
    
    int num_child;
    
    if(argc > 1){
        num_child = atoi(argv[1]);
        if(num_child<1 || num_child>60){
            fprintf(stderr, "Invalid Input\n");
        }
        int *pid = (int *)malloc(sizeof(int)*num_child);

        for(int i=0; i<num_child; i++){
            
            switch(pid[i] = fork()){
                case 1:
                    fprintf(stderr, "fork failed\n");
                    exit(1);
                case 0:
                    while(1){
                        printf("%d\n", i);
                        sleep(5);
                    }
                default:
                    //printf("pid : %d\n", pid[i]);
                    break;
            }
        }

        sleep(5);

        for(int i=num_child-1; i>=0; i--){
            kill(pid[i], SIGKILL);
            printf("%d번째 Process[%d] 종료\n", i, pid[i]);
            sleep(1);
        }

        printf("프로그램 종료\n");
        raise(SIGKILL);

    }else{
        fprintf(stderr, "myparent (number)");
        exit(1);
    }
}